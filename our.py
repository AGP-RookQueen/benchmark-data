import gurobipy as gp

# Refer to the paper: [my thesis]

TIMEOUT_IN_HOURS = 1

def get_identifiers(r, c, dr, dc):
  assert(matrix[r][c] == '#')
  ids = []
  while (r >= 0 and r < rows and c >= 0 and c < cols and matrix[r][c] == '#'):
    ids.append(identifiers[r][c])
    r += dr
    c += dc
  return ids

def get_positions(r, c, dr, dc):
  assert(matrix[r][c] == '#')
  positions = []
  while (r >= 0 and r < rows and c >= 0 and c < cols and matrix[r][c] == '#'):
    positions.append( (r, c) )
    r += dr
    c += dc
  return positions

# If SEGMENT is NOT row-covered ==> every cell in it is covered
def condition1(row, col):
  cells = list(set( get_identifiers(row, col, +1, 0) + get_identifiers(row, col, -1, 0) ))
  segment_id = identifiers_of_segments[row][col]
  model.addConstr(sum(x[i] for i in cells) >= (1 - c[segment_id]))


def condition2and3(row, col):
  
  positions = list(set( get_positions(i, j, +1, 0) + get_positions(i, j, -1, 0) ))
  segment_id = identifiers_of_segments[row][col]

  region = []
  rows_in_segment = 0
  for (n_row, n_col) in positions:
    assert(n_col == col)
    assert(matrix[n_row][n_col] == '#')
    
    rows_in_segment += 1

    cells = list(set( get_identifiers(n_row, n_col, 0, +1) + get_identifiers(n_row, n_col, 0, -1) ))
    region = region + cells

    # Row-covered => A rook per row
    model.addConstr(sum(x[i] for i in cells) >= c[segment_id])
  
  assert(rows_in_segment == len(positions))
  # Se não está row-covered, então não pode existir uma torre por linha
  model.addConstr(sum(x[i] for i in region) <= rows_in_segment - 1 + c[segment_id])


# Create a new Gurobi model
model = gp.Model()

# Set model parameters
model.setParam('OutputFlag', 0)  # Turn off solver output
model.setParam('TimeLimit', TIMEOUT_IN_HOURS * 1) # Set time limit for solver in hours
model.setParam('LogToConsole', 0)  # Show solver progress

# Read number of rows and cols
rows, cols = map(int,input().split(" "))

matrix = []
identifiers = []

number_of_cells = 0

for i in range(rows):
  row = input()
  matrix.append(list(row))
  identifiers.append([-1] * cols)
  number_of_cells += row.count('#')

# Create the decision variables: For each cell i, x_i = 1 iff it contains a rook
x = model.addVars(number_of_cells, vtype='I')

# Assign each tile an unique ID
id = 0
for i in range(rows):
  for j in range(cols):
    if (matrix[i][j] == '#'):
      identifiers[i][j] = id
      id += 1

assert(id == number_of_cells) # There were assigned as many ids as the number of cells

identifiers_of_segments = [ [-1] * cols for _ in range(rows) ]
segment_id = 0

for i in range(rows):
  for j in range(cols):
    if (matrix[i][j] == '.'):
      continue
    if (identifiers_of_segments[i][j] != -1):
      continue

    positions = list(set( get_positions(i, j, +1, 0) + get_positions(i, j, -1, 0) ))

    for (row, col) in positions:
      identifiers_of_segments[row][col] = segment_id

    segment_id += 1
     
number_of_segments = segment_id

c = model.addVars(number_of_segments, vtype="I")               # Set of row-covered columns

cell_visited = [ [0] * cols for _ in range(rows) ]

for i in range(rows):
  for j in range(cols):
    if (matrix[i][j] == '.'):
      continue
    if (cell_visited[i][j]):
      continue
    
    positions = list(set( get_positions(i, j, +1, 0) + get_positions(i, j, -1, 0) ))

    for (row, col) in positions:
      assert(col == j)
      assert(cell_visited[row][col] == 0)
      cell_visited[row][col] = 1
      
    condition1(i, j)
    condition2and3(i, j)


# Non-attacking rooks
for i in range(rows):
  for j in range(cols) :
    if (matrix[i][j] == '.'):
      continue

    cells_in_row = list(set( get_identifiers(i, j, +0, +1) + get_identifiers(i, j, +0, -1) ) )
    cells_in_col = list(set( get_identifiers(i, j, +1, +0) + get_identifiers(i, j, -1, +0) ) )

    current_cell_id = identifiers[i][j]
    
    total = cells_in_row + cells_in_col

    for cell_id in cells_in_row:
      if (cell_id != current_cell_id):
        model.addConstr( x[current_cell_id] + x[ cell_id ] <= 1)

    for cell_id in cells_in_col:
      if (cell_id != current_cell_id):
        model.addConstr( x[current_cell_id] + x[ cell_id ] <= 1)


# Set the objective: minimise the total number of rooks
model.setObjective(sum(x[i] for i in range(0, number_of_cells)), gp.GRB.MINIMIZE)

# Optimise!
model.optimize()

# Report answer obtained
print("Optimal answer:", round(model.objVal))
print("Runtime:", model.Runtime)
print("Status:", model.Status)
print(round(model.objVal), model.Runtime, model.Status)