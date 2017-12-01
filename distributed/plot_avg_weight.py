import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter

def main():
    filename = 'result.txt'
    value = []
    x = []
    y = []
    with open(filename) as fptr:
        for line in fptr:
            value.append(line.strip().split(' '))
    
    for pair in value:
        x.append(float(pair[0]))
        y.append(float(pair[1]))

    fig, ax = plt.subplots()
    ax.yaxis.set_major_formatter(FormatStrFormatter('%.2f'))
    
    plt.xlabel('Probability')
    plt.ylabel('Averaged total weight')
    
    plt.plot(x, y, 'bo')
    plt.plot(x, y, label = 'averaged total weight')

    plt.title("Averaged convergence time changes with the setting of p")
    plt.legend()

    plt.savefig("avg_weight.png")

# Main function
if __name__ == '__main__':
    main()