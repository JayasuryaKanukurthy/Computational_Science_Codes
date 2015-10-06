#!/usr/bin/python

""" Particle-Mesh Simulation Code:
Author: Jayasurya

Description: Simulates the movement of charged particles on an electrostatically
             charged grid under applied and mutual charged potential fields.

             Script relies on Numpy and Matplotlib libraries for 
             numerical calculations and visualization.
             
             Usage: 
                 $python Particle_Mesh_Simulation.py
            
Date:  05/3/15

"""

__author__="Jayasurya Kanukurthy"


import numpy as np
from numpy.random import uniform as random
import matplotlib.pyplot as plot
from math import sqrt
import matplotlib.animation as animation

class ParticleMesh(object):
    """This class is used to generate a Mesh with a set of randomly initialized 
       particles. 
       
        1. Computationally model forces boundary conditions on the left 
           and right boundaries. The top and bottom boundaries are kept free to 
           change (Neumann boundary condition). Each of the random particles
           contribute charge density to the grid at a point nearest to them.
          
        2. Numerically, we solve the charge density and applied boundary conditions
           by solving Poisson equation for electrostatics:
               
                    (d^2 x)/dx^2 + (d^2 y)/dy^2 = -q/e
            
            Script uses Successive Over Relaxation (SOR) numerical method for this.
            
        3. At each time step - a. solve the charge density distribution on grid 
          (as in 2. above), b. calculate the gradients and forces acting on particles 
          due to this, and c. compute the new position of the particles using this force.
          
        Attributes: size - tuple(int,int)::gives size of the grid,
                    num_particles - int::number of particles
                    left_boundary/right_boundary - float:: boundary values
        """
    
    def __init__(self, size=(500,500),num_particles=200,left_boundary=-50,right_boundary=50):
        
        """Initializer: Initializing mesh and random particles"""
        
        self.size = size
        self.particles = [Particles(box=(self.size[0],self.size[1]), 
                                    max_charge=5,max_mass=5) \
                            for i in xrange(num_particles)]
        self.left_boundary = left_boundary
        self.right_boundary = right_boundary
        self.grid_potential =  np.zeros(size)
        self.grid_potential = self.solve_grid_poisson()
        self.particle_paths = [(self.x,self.y)]
        

    def __repr__(self): 
        return '{}(size={},particles={},boundary conditions(left,right) = {}'.format(
                self.__class__.__name__, self.size, len(self.particles), 
                (self.left_boundary,self.right_boundary))
    
    def __str__(self):
        return 'Canvas for our particle mesh simulation:'+'\n'+self.__repr__()
    
        
    #########      Defining Attributes and Properties     ##############    
    
    @property
    def x(self):
        return [p.position[0] for p in self.particles]

    @property
    def y(self):
        return [p.position[1] for p in self.particles]
    
    @property
    def xy(self):
        xy=[]
        for p in self.particles:
            xy.append([p.position[0],p.position[1]])
        
        return xy
    
    @property
    def charge_sign(self):
        red = "red"             #red for +ve charge
        blue = "blue"           #blue for -ve charge
        
        return [(red if p.charge >0 else blue) for p in self.particles]
    
    @property
    def charge(self):
        return [p.charge for p in self.particles]

    @property
    def mass(self):
        return [p.mass*20 for p in self.particles]
   
        
     #############     Defining methods     ############
     
    def advance_time(self,steps=1):
        '''Advances the simulation for given timesteps (default = 1)'''
        for x in xrange(steps):
            self.solve_grid_poisson()
            self.calculate_forces()
            self.move_particles()
            self.save_particle_positions()  #save if requried for plotting later on
    

    def solve_grid_poisson(self, covergence_factor=0.01):
        '''Solves the poisson equation on grid for potential'''
        
        potential=np.zeros(self.size,dtype=float)
        charge=np.zeros(self.size,dtype=float)
        xdim = self.size[0]                           # index of X Boundary 
        ydim = self.size[1]                           # index of Y Boundary 
        
        
        #####     Placing particle charges at relevant grid points     
        
        for p in self.particles:
            try:
                ###  placing the charge at its nearest grid point.
                
                charge[int(round(p.position[0])), int(round(p.position[1]))] = p.charge 
                
            except IndexError:
                ###   Out of bounds error: occurs when particles are out of the ROI
                continue


        ######    Applying Boundary Conditions on left and right ends    
        
        potential[:,0] = self.left_boundary
        potential[:,xdim-1] = self.right_boundary
        
        ######   Applying Neumann Boundary Conditions at top and bottom boundary
        
        for i in xrange(1,ydim):
            eta = (float(i)/self.size[1])
            potential[0,i] = potential[0,i] + (1-eta)*self.left_boundary + eta*self.right_boundary
            potential[ydim-1,i] = potential[ydim-1,i] + (1-eta)*self.left_boundary \
                                              + eta*self.right_boundary
                                              
        phi = potential  #Just renaming variables
        rho = charge        
        
        #####   Solving the charge distribution on grid for potential field
        
        a = 1; b = 1; c = -4; omega = 0.8     #SOR parameters
        
        sum_norm = 3.0

        while(sum_norm > covergence_factor):
            sum_norm = 0.0
            for i in xrange(1,xdim-1):
                for j in xrange(1,ydim-1):
                    phi_above = phi[i-1,j]
                    phi_below = phi[i+1,j]
                    phi_left = phi[i,j-1]
                    phi_right = phi[i,j+1]                    
                    Rij = (a*(phi_left+phi_right)+b*(phi_above+phi_below)+c*phi[i][j] - rho[i][j])/c; 
                    phi[i,j] = phi[i,j]-omega*Rij
                    
                    if(phi[i,j] <> 0.0):
                        sum_norm = sum_norm + Rij*Rij
            sum_norm = sqrt(sum_norm)/(xdim*ydim)
        
        self.grid_potential = potential
  
    
        
    def calculate_forces(self):
        
        '''Calculates the forces experienced by particles at the current time step.
            The forces are obtained by finding the potential gradients around the 
            particle. Force is obtained multiplying gradient and -ve of particle
            charge.
            Gradient iscalculated using first order central difference.
            
        '''
        
        grid = self.grid_potential
        
        for p in self.particles:
            xpos = int(round(p.position[0]))
            ypos = int(round(p.position[1]))
            if (xpos > 1 and xpos < self.size[0]-2 and ypos > 1 and ypos < self.size[1]-2):
                gradientx = (grid[xpos-1,ypos]-grid[xpos+1,ypos])/2.0
                gradienty = (grid[xpos,ypos-1]-grid[xpos,ypos+1])/2.0
            else:
                continue
            p.set_force((gradientx,gradienty))

    def move_particles(self):
        
        
        '''Move the particles on the grid using Verlet's algorithm.
           Just a fancy way of saying 
           a = F/m
           v = x1-x0
           x = x0 + v*t+a*t^2
           
           elementary physics :)
           
           '''
        for p in self.particles:
            x0 = p.position0[0]
            y0 = p.position0[1]
            x1 = p.position[0]
            y1 = p.position[1]
            
            ax = p.forcex/p.mass
            ay = p.forcey/p.mass
            
            dt = 1
            x_new = x1+dt*(x1-x0)+dt*dt*ax
            y_new = y1+dt*(y1-y0)+dt*dt*ay
            
            if (x_new < 1 or x_new > self.size[0] or y_new < 1 or y_new > self.size[1]):
                p.set_position(-1,-1)
            elif(x_new<0 or y_new<0):
                continue
            else:
                p.set_position(x_new,y_new)
        
        return
    
    def save_particle_positions(self):
        self.particle_paths.append((self.x,self.y))

class Particles(object):
    """Charged Particles attributed with Mass, Charge, Initial Position, and 
       Forces along (x, y).
       
    """
    
    def __init__(self, box=(500,500), max_charge=25, max_mass=10):
        self.position = (random()*box[0],random()*box[1])
        self.position0 = self.position
        self.charge = random(-1,1)*max_charge
        self.mass = random()*max_mass
        self.forcex = 0
        self.forcey = 0
    
    def __repr__(self):
        return '{}(position={},charge={},mass={})'.format(self.__class__.__name__,self.position, 
                                                           self.charge, self.mass)
    def set_position(self,x,y):
        self.position0 = self.position
        self.position = (x,y)
        
    def set_force(self,gradient=(0,0)):
        self.forcex = -self.charge*gradient[0]
        self.forcey = -self.charge*gradient[1]


def update_plot(i,scat,pm):
    ''' Updates the Artists to use for animation '''
    pm.advance_time()
    t=[pm.x,pm.y]
    ofst = []
    for i in xrange(len(t[0])):
        ofst.append([t[0][i],t[1][i]])
    ofst = tuple(ofst)
    scat.set_offsets(ofst)
    
    return scat,
    


pm = ParticleMesh()

fig = plot.figure()

ax = fig.add_subplot(111)
ax.grid(True,linestyle='-',color='0.75')
ax.set_xlim([0,500])
ax.set_ylim([0,500])

scat = plot.scatter(pm.x, pm.y, s = pm.mass,c=pm.charge_sign)
anim = animation.FuncAnimation(fig,update_plot,fargs=(scat,pm))
plot.show()
