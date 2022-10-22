import matplotlib.pyplot as plt

# read out moisture data
datafile = open("./testlist", "r")
data = datafile.readlines()
datafile.close()
data = [int(sub.replace("\n", "")) for sub in data]

# simple plot of the data
plt.plot(data)
plt.show()
