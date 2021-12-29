from matplotlib import pyplot as plt
def main():
  aR = []
  bR = []
  aS = []
  bS = []

  with open("hist_data_r.txt") as fr:
    linesr = fr.readlines()
  for lr in linesr:
    i1r,i2r = lr.strip().split('=>')
    aR.append((int)(i1r))
    bR.append((int)(i2r))

  with open("hist_data_s.txt") as fs:
    liness = fs.readlines()
  for ls in liness:
    i1s,i2s = ls.strip().split('=>')
    aS.append((int)(i1s))
    bS.append((int)(i2s))

  plt.style.use('ggplot')
  plt.figure(1)
  plt.bar(aR,bR)
  plt.xlim((-1800,1800))
  plt.ylim((0,75000))

  plt.style.use('ggplot')
  plt.figure(2)
  plt.bar(aS,bS)
  plt.xlim((-7500,7500))
  plt.ylim((0,7500))


  #plt.hist()
  plt.show()
if __name__ == "__main__":
  main()