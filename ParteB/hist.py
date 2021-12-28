from matplotlib import pyplot as plt
def main():
  a = []
  b = []

  with open("hist_data.txt") as f:
    lines = f.readlines()
  for l in lines:
    i1,i2 = l.strip().split('=>')
    a.append((int)(i1))
    b.append((int)(i2))

  plt.style.use('ggplot')
  plt.figure(1)
  plt.bar(a,b)
  #plt.xlim((-2000,2000))
  #plt.hist()
  plt.show()
if __name__ == "__main__":
  main()