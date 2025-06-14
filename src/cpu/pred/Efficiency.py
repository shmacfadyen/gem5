import matplotlib.pyplot as plt

# make sure you sudo apt install python3-matplotlib 

def data_getter():
    '''
    data_getter gets the number of correctly and incorrectly perdicted attempts 
    returns miss & hit perdiction #
    '''
    with open('/home/mattz/gem5/mytest_output/stats.txt', 'r') as stats:
        # gets the correctly predicted #, system.cpu.branchPred.condPredicted
        for line in stats:
            if "system.cpu.branchPred.condPredicted" in line:  
                hit = int(line.split()[1])
                print(hit)
                break
        # gets the incorrectly perdicted #, system.cpu.branchPred.condIncorrect
        for line in stats:
            if "system.cpu.branchPred.condIncorrect" in line:
                miss = int(line.split()[1])
                print(miss)
                break
    return miss, hit
    
def calculate():
    '''
    calculate gets percentage perdiction accuracy
    returns percent_hit
    '''  
    miss, hit = data_getter()
    percent_hit = 100*(1- miss/hit)
    print(percent_hit)
    return percent_hit
    
def plot():  
    '''
    this is used to plot the accuracy of the  perdiction
    '''
    percent_hit = calculate()
    
    GAg = 91.1
    GAp = 92.3
    PAg = 93.3
    PAp = 94.4
    Gshare = 95.5
    Perceptron = 96.6
    O_GEHL = 97.7
    
    x = ["GAg", "GAp", "PAg", "PAp", "Gshare", "Perceptron", "O-GEHL"]
    y = [GAg, GAp, PAg, PAp, Gshare, Perceptron, O_GEHL]
    #y = [percent_hit] 
    
    plt.bar(x, y, color = 'red')
    plt.title("Accuracy Plot")
    plt.xlabel("Branch Perdiction Type")
    plt.ylabel("Perdiction Accuracy (%)")
    plt.grid(True)
    plt.savefig("Plots.png")

if __name__ == "__main__":
    plot()

