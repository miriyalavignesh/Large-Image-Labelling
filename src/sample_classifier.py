import graphlab as graphlab
data = graphlab.SFrame.read_csv('sample_data_label', delimiter='   ', header=False)
feature_label = []
for i in range(1,60):
	feature_label.append("X"+str(i))	
model = graphlab.svm.create(data, target='X60', features=feature_label)
print "coefficients::"+ str(model['coefficients'])

