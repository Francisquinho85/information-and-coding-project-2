from matplotlib import pyplot as plt
def main():
    a = []
    b = []
    with open("samp.txt") as f:
        lines = f.readlines()
    for l in lines:
        i1,i2 = l.strip().split("_")
        a.append(i1)
        b.append(i2)
    plt.style.use('ggplot')
    plt.bar(a,b)
    plt.show()

if __name__ == "__main__":
    main()