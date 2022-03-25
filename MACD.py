# Tools for csv manipulation
import csv
# Tools for date strings manipulation
from datetime import datetime
# Tools for data visualisation
import matplotlib.pyplot as plt
# Tools for changing the legend of the plot
import matplotlib.patches as mpatches


# Function calculating EMAn
def EMAn(periodNumber, values, sampleNumber):
    alfa = 2 / (N + 1)
    number = 1 - alfa
    numerator = 0.0
    denominator = 0.0
    for i in range(periodNumber + 1):
        numerator += pow(number, i - 1) * values[sampleNumber - i - 1]
        denominator += pow(number, i - 1)
    return numerator / denominator


# Function calculating MACD
def MACD(values, sampleNumber):
    return EMAn(12, values, sampleNumber) - EMAn(26, values, sampleNumber)


# Function calculating SIGNAL
def Signal(values, sampleNumber):
    return EMAn(9, values, sampleNumber)


# Function that buys stocks. I consider that the number of stocks bought can be a float - fractional shares
def Buy(balance_before, money_before, exchangerate):
    balance_after = money_before / exchangerate
    balance_after += balance_before
    money_after = 0.0
    return balance_after, money_after


# Function that sells stocks. I consider that the number of stocks sold can be a float - franctional shares
def Sell(balance_before, money_after, exchangerate):
    balance_after = 0.0
    money_after += balance_before * exchangerate
    return balance_after, money_after


def MACD_Investment(start_money, MACD_Save, SIGNAL_Save, closing_values):
    stocks = 0.0
    money = start_money
    for i in range(1, N):
        # When it's time to buy - the exchange rate will rise
        if MACD_Save[i - 1] < SIGNAL_Save[i - 1] and MACD_Save[i] >= SIGNAL_Save[i]:
            stocks, money = Buy(stocks, money, closing_values[i])
        elif MACD_Save[i - 1] > SIGNAL_Save[i - 1] and MACD_Save[i] <= SIGNAL_Save[i]:
            stocks, money = Sell(stocks, money, closing_values[i])
    if money == 0:
        stocks, money = Sell(stocks, money, closing_values[len(closing_values) - 1])
    return money


def Delayed_MACD_Investment(start_money, MACD_Save, SIGNAL_Save, closing_values):
    stocks = 0.0
    money = start_money
    earlier = 5
    for i in range(earlier, N - 1):
        # When it's time to buy - the exchange rate will rise
        if MACD_Save[i - earlier] < SIGNAL_Save[i - earlier] and MACD_Save[i] >= SIGNAL_Save[i]:
            stocks, money = Buy(stocks, money, closing_values[i])
        elif MACD_Save[i - earlier] > SIGNAL_Save[i - earlier] and MACD_Save[i] <= SIGNAL_Save[i]:
            stocks, money = Sell(stocks, money, closing_values[i])
    if money == 0:
        stocks, money = Sell(stocks, money, closing_values[len(closing_values) - 1])
    return money


# Function to load the data
def FileLoad(name):
    file = open(name)
    csvreader = csv.reader(file)
    next(csvreader)  # We ignore titles of columns
    closing_values = []
    dates = []
    # We load all the data in one list. Each element consist of: Date, Opening, Min, Max, Closing, Volume.
    # In the task I will base on closing rate.
    for row in csvreader:
        # We only want closing rate - that's fifth element
        closing_values.append(float(row[4]))
        dates.append(row[0])
    file.close()
    return dates, closing_values

def DataSave(name, x_values, MACD_Save, SIGNAL_Save):
    f = open(name, "w")
    for i in range(N):
        f.write(str(x_values[i]))
        f.write(",")
        f.write(str(MACD_Save[i]))
        f.write(",")
        f.write(str(SIGNAL_Save[i]))
        f.write("\n")
    f.close()

dates, closing_values = FileLoad('data.csv')

# Readings count
N = 1000
# Beginning balance
beg_balance = 1000.0
# Series save locations
MACD_Save = []
SIGNAL_Save = []
for i in range(N):
    MACD_Save.append(MACD(closing_values, i))
for i in range(N):
    SIGNAL_Save.append(Signal(MACD_Save, i))

x_values = [datetime.strptime(d, "%Y-%m-%d").date() for d in dates]

DataSave("Data_save.txt", dates,  MACD_Save, SIGNAL_Save)

# Plot showing closing rate in last 1000 days
plt.figure(figsize=(12, 5))
plt.plot(x_values, closing_values, label='Closing values')
plt.xlabel('Date of reading')
plt.ylabel('Closing value')
plt.legend()
plt.title("Last 1000 closing values of WIG20")
plt.savefig("closing_rate.png", bbox_inches='tight', dpi=1200)
plt.close()

# MACD + SIGNAL plot
plt.figure(figsize=(12, 5))
plt.plot(x_values, MACD_Save, label="MACD")
plt.xlabel('Date of reading')
plt.ylabel('Value of MACD indicator')
plt.plot(x_values, SIGNAL_Save, label="SIGNAL")
for i in range(1, N - 1):
    if MACD_Save[i - 1] < SIGNAL_Save[i - 1] and MACD_Save[i] >= SIGNAL_Save[i]:
        plt.scatter(x_values[i], MACD_Save[i] - 10, color="green", label="Buy", marker='^', alpha=1)
    if MACD_Save[i - 1] > SIGNAL_Save[i - 1] and MACD_Save[i] <= SIGNAL_Save[i]:
        plt.scatter(x_values[i], MACD_Save[i] + 10, color="red", label="Sell", marker='v', alpha=1)
legend_dict = {"MACD": "blue", "Signal": "orange", "Buy": "green", "Sell": "red"}
patchList = []
for key in legend_dict:
    data_key = mpatches.Patch(color=legend_dict[key], label=key)
    patchList.append(data_key)
plt.legend(handles=patchList)
plt.title("Buy and sell moments with Signal and MACD")
plt.savefig("macd+signal.png", bbox_inches='tight', dpi=1200)
plt.close()

# Buy and sell with prices
plt.figure(figsize=(12, 5))
plt.plot(x_values, closing_values, label="Closing value")
plt.xlabel('Date of reading')
plt.ylabel('Closing value')
for i in range(1, N - 1):
    if MACD_Save[i - 1] <= SIGNAL_Save[i - 1] and MACD_Save[i] >= SIGNAL_Save[i]:
        plt.scatter(x_values[i], closing_values[i] - 50, color="green", label="Buy", marker='^', alpha=1)
    elif MACD_Save[i - 1] >= SIGNAL_Save[i - 1] and MACD_Save[i] <= SIGNAL_Save[i]:
        plt.scatter(x_values[i], closing_values[i] + 50, color="red", label="Sell", marker='v', alpha=1)
legend_dict2 = {"Closing value": "blue", "Buy": "green", "Sell": "red"}
patchList2 = []
for key in legend_dict2:
    data_key = mpatches.Patch(color=legend_dict2[key], label=key)
    patchList2.append(data_key)
plt.legend(handles=patchList2)
plt.title("Buy and sell moments with closing values")
plt.savefig("MACD + closing_values.png", bbox_inches='tight', dpi=1200)
plt.close()

# Transaction - only based on MACD
print("Only MACD + SIGNAL crossings. Beginning balance was 1000. Now you have",
      round(MACD_Investment(beg_balance, MACD_Save, SIGNAL_Save, closing_values)))
print("Only MACD + SIGNAL crossings, but with two day delay. Beginning balance was 1000. Now you have",
      round(Delayed_MACD_Investment(beg_balance, MACD_Save, SIGNAL_Save, closing_values)))
