from pylab import *
a=loadtxt('../../gauss_direction.txt')
x=a[:,0]
y=a[:,1]
print len(x),len(y)

def f(x,y,a,b):
    return x*(x*a[0,0]+y*a[1][0])+y*(x*a[0][1]+y*a[1][1])-b[0]*x-b[1]*y


def PLOT(A,b,ax):
    aa=20
    bb=20
    x=linspace(-aa,bb,aa*20)
#    print 'x',x
#    y=linspace(-aa,bb,aa*20)
#    X,Y=meshgrid(x,y)
#    z=array([f(x,y,A,b) for x,y in zip(ravel(X),ravel(Y))])
#    Z=z.reshape(X.shape)
#    ax.imshow(flipud(Z),cmap=cm.jet,extent=[-20, 20, -20, 20])
    ax.plot(x,1-x*3/2,'k',linewidth=1)
    print 1-x*3/2
    ax.plot(x,-8./6-x*2/6,'k',linewidth=1)
    ax.grid('on')


#    cset = ax.contour(X, Y, Z, zdir='z',offset=Z.min()-800, cmap=cm.jet)
#    ax.set_zlim3d(Z.min()-800, Z.max());


A=array([[3,2],[2,6]])
b=array([2,-8])

fig=figure()
ax=fig.add_subplot(111)
PLOT(A,b,ax)
for i in range(len(x)-1):
    print x[i:i+2],y[i:i+2]
    ax.plot(x[i:i+2],y[i:i+2],'r',linewidth=2)
#ax.set_xticks([])
#ax.set_yticks([])
ax.set_ylim([-10,10])
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_title('Gauss-Seidel Iterations Example',fontsize=22)
show()


