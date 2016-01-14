from pylab import *

sparse=loadtxt('matrix_vector_sparse.txt')
plain=loadtxt('matrix_vector_plain.txt')
X=sparse[:,0]
sparse=sparse[:,1]
plain=plain[:,1]
print X
fig=figure()
ax=fig.add_subplot(111)
ax.plot(X,sparse,'red')
ax.plot(X,plain,'blue')
ax.legend(["Sparse Matrix","Plain Matrix"],loc=2)
ax.set_ylabel("Time in sec.")
ax.set_xlabel("Size of the matrix")
ax.set_title('Matrix*Vector computation time, 10% sparsity')
ax.grid('on')
show()


