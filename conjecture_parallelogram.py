import gurobipy as gp

# Refer to the paper: https://link.springer.com/content/pdf/10.1007/s40687-024-00492-5.pdf
# and GitHub repository: https://github.com/PhoenixSmaug/Polyomino.jl/tree/main

TIMEOUT_IN_HOURS = 1

def get_identifiers(r, c, dr, dc):
  assert(matrix[r][c] == '#')
  ids = []
  while (r >= 0 and r < rows and c >= 0 and c < cols and matrix[r][c] == '#'):
    ids.append(identifiers[r][c])
    r += dr
    c += dc
  return ids

# Create a new Gurobi model
model = gp.Model()

# Set model parameters
model.setParam('OutputFlag', 1)  # Turn off solver output
model.setParam('TimeLimit', TIMEOUT_IN_HOURS * 3600) # Set time limit for solver in hours
model.setParam('LogToConsole', 1)  # Show solver progress

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

# All Domination Constrains
for i in range(rows):
  for j in range(cols):
    if (matrix[i][j] == '.'):
      continue
    
    cells_in_row = list(set( get_identifiers(i, j, +0, +1) + get_identifiers(i, j, +0, -1) ) )
    cells_in_col = list(set( get_identifiers(i, j, +1, +0) + get_identifiers(i, j, -1, +0) ) )
    cells_visible = list(set(cells_in_row + cells_in_col))

    assert(len(cells_visible) == len(cells_in_row) + len(cells_in_col) - 1)

    model.addConstr(sum(x[i] for i in cells_in_row) <= 1)
   # model.addConstr(sum(x[i] for i in cells_in_col) <= 1) -- I allow multiple rooks in the same column
    model.addConstr(sum(x[i] for i in cells_visible) >= 1)
    
# Add my conjecture
for i in range(rows):
  for j in range(cols):
    if (matrix[i][j] == '.'):
      continue

    for ii in range(rows):
      for jj in range(cols):
        if (matrix[ii][jj] == '.'):
          continue
        if (ii <= i):
          continue
        if (jj <= j):
          id_above = identifiers[i][j]
          id_here = identifiers[ii][jj]
          model.addConstr(x[id_above] + x[id_here] <= 1)
# Set the objective: minimise the total number of rooks
model.setObjective(sum(x[i] for i in range(0, number_of_cells)), gp.GRB.MINIMIZE)



# Optimise!
model.optimize()

# Report answer obtained
print("Optimal answer:", round(model.objVal))
print("Runtime:", model.Runtime)
print("Status:", model.Status)
print(round(model.objVal), model.Runtime, model.Status)