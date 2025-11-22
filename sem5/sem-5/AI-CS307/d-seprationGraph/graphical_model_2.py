from pgmpy.models import BayesianNetwork

# Create the Bayesian Network
model = BayesianNetwork([('A', 'C'), ('B', 'C'), ('C', 'D')])

# Check d-separation using active trail nodes
active_trail_nodes = model.active_trail_nodes('A', observed={'C'})
is_independent = 'B' not in active_trail_nodes['A']

print(f"A and B are independent given C: {is_independent}")