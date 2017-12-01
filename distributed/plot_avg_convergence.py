import matplotlib.pyplot as plt

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
        y.append(int(pair[2]))
    
    plt.xlabel('Probability')
    plt.ylabel('Averaged convergence time')

    plt.plot(x, y, 'bo')
    plt.plot(x, y, label = 'averaged covergence time')

    plt.title("Averaged convergence time changes with the setting of p")
    plt.legend()
    
    plt.savefig("avg_convergence.png")

# Main function
if __name__ == '__main__':
    main()