import pandas as pd
from pgmpy.models import BayesianNetwork
from pgmpy.estimators import HillClimbSearch, BicScore
from pgmpy.estimators import MaximumLikelihoodEstimator
from pgmpy.inference import VariableElimination

# Load the data
data_df = pd.read_csv('data.txt', sep='\t')

# Define grade mappings
grade_to_int = {
    'AA': 10, 'AB': 9, 'BB': 8, 'BC': 7, 
    'CC': 6, 'CD': 5, 'DD': 4, 'FF': 0
}
int_to_grade = {v: k for k, v in grade_to_int.items()}

# Drop the 'QP' column
data_df = data_df.drop('QP', axis=1)

# Replace grades with integers
data_df = data_df.replace(grade_to_int)

# Ensure all columns are of integer type
data_df = data_df.apply(pd.to_numeric, errors='coerce').fillna(0).astype(int)

# Learn the structure of the Bayesian Network
hc = HillClimbSearch(data_df)
best_model = hc.estimate(scoring_method=BicScore(data_df))
print(best_model.edges())

# Create the Bayesian Network with the learned structure
# model = BayesianNetwork(best_model.edges())

# Fit the data to the model to learn the CPTs
# model.fit(data_df, estimator=MaximumLikelihoodEstimator)

# # Print specific CPDs for the relationships between PH100 and MA101, EC100 and MA101, IT101 and MA101
# print("CPD between PH100 and MA101:")
# cpd_ph100_ma101 = model.get_cpds('PH100')
# print(cpd_ph100_ma101)

# print("\nCPD between EC100 and MA101:")
# cpd_ec100_ma101 = model.get_cpds('EC100')
# print(cpd_ec100_ma101)

# print("\nCPD between IT101 and MA101:")
# cpd_it101_ma101 = model.get_cpds('IT101')
# print(cpd_it101_ma101)

# # Perform inference
# inference = VariableElimination(model)

# # Evidence: Grades in EC100, IT101, and MA101
# evidence = {
#     'EC100': grade_to_int['DD'],
#     'IT101': grade_to_int['CC'],
#     'MA101': grade_to_int['CD']
# }

# # Query: Probability distribution for PH100
# result = inference.map_query(variables=['PH100'], evidence=evidence)

# # Convert the result back to grade format
# predicted_grade = int_to_grade[result['PH100']]
# print(f"\nPredicted grade for PH100: {predicted_grade}")
