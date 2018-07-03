import numpy as np

#
class GN_temp():
  #
  def __init__(self):
    self.data   = None
    self.t      = None
    self.dt     = None
    self.pars   = None
    self.Dpar   = None

  # 
  def setup(self, data, t, pars, dt = 0.001):
    self.data   = data
    self.t      = t
    self.dt     = dt
    self.pars   = pars
    self.Dpar   = dt*np.eye(len(pars))

  #
  def h(self, p):
    nad = p[0] + self.t ** p[1]
    return nad - self.data
#
#  Ex:
#     nad = p[0]*np.exp(p[0]*self.t) + p[1]
#     return nad - self.data
#

  #
  def get_jacobian(self):
    J = np.zeros(len(self.pars)*len(self.data)).reshape(len(self.data), len(self.pars))
    for index in range(len(self.pars)):
      fad = self.Dpar[index, :]
      J[:, index] = (self.h(self.pars + fad) - self.h(self.pars - fad))/(2*self.dt)
    return J

  #
  def modify_distance(self, dp):
    t = 1
    nad = sum(self.h(self.pars) ** 2)
    fad = sum(self.h(self.pars + t*dp) ** 2)
    while (fad > nad):
      if (t < 10**(-10)):
        print(' Warning: Bad convergence, t-parameter < e-10')
        return t
      t = t/2
      fad = sum(self.h(self.pars + t*dp) ** 2)
    return t
      
  #
  def step(self):
    J = self.get_jacobian()
    dp = np.linalg.lstsq(J, -self.h(self.pars), rcond=None)
    t = self.modify_distance(dp[0])
    self.pars += t*dp[0]

  #
  def optimize(self, reps):
    for rep in range(reps):
      self.step()
    return self.pars


