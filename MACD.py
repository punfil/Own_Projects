# Tools for csv manipulation
import csv
# Tools for date strings manipulation
from datetime import datetime
# Tools for
import matplotlib.pyplot as plt
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


# Function that buys stocks. I consider that the number of stocks bought can only be an integer
def Buy(balance_before, money_before, exchangerate):
    balance_after = money_before / exchangerate
    balance_after += balance_before
    money_after = 0
    return balance_after, money_after


# Function that sells stocks. I consider that the number of stocks sold can only be an integer
def Sell(balance_before, money_after, exchangerate):
    balance_after = 0
    money_after += balance_before * exchangerate
    return balance_after, money_after


def SMA(values, begin, sampleNumber):
    sum = 0
    for i in range(begin, begin + sampleNumber):
        sum += values[i]
    return sum / sampleNumber


def MACD_Investment(start_money, MACD_Save, SIGNAL_Save, closing_values):
    stocks = 0.0
    money = start_money
    for i in range(N - 1):
        # When it's time to buy - the exchange rate will rise
        if MACD_Save[i] < SIGNAL_Save[i] and MACD_Save[i + 1] >= SIGNAL_Save[i + 1]:
            stocks, money = Buy(stocks, money, closing_values[i])
        elif MACD_Save[i] > SIGNAL_Save[i] and MACD_Save[i + 1] <= SIGNAL_Save[i + 1]:
            stocks, money = Sell(stocks, money, closing_values[i])
    if money == 0:
        stocks, money = Sell(stocks, money, closing_values[len(closing_values) - 1])
    return money


def Delayed_MACD_Investment(start_money, MACD_Save, SIGNAL_Save, closing_values):
    stocks = 0.0
    money = start_money
    for i in range(2,N - 1):
        # When it's time to buy - the exchange rate will rise
        if MACD_Save[i-2] < SIGNAL_Save[i] and MACD_Save[i] >= SIGNAL_Save[i]:
            stocks, money = Buy(stocks, money, closing_values[i])
        elif MACD_Save[i-2] > SIGNAL_Save[i] and MACD_Save[i] <= SIGNAL_Save[i]:
            stocks, money = Sell(stocks, money, closing_values[i])
    if money == 0:
        stocks, money = Sell(stocks, money, closing_values[len(closing_values) - 1])
    return money


def SMA_Investment(start_money, closing_values):
    last_SMA30 = 0
    last_SMA100 = 0
    stocks = 0.0
    money = start_money
    for i in range(100, N - 1):
        SMA30 = SMA(closing_values, i - 30, 30)
        SMA100 = SMA(closing_values, i - 100, 100)
        if last_SMA30 < last_SMA100 and SMA30 >= SMA100:
            stocks, money = Buy(stocks, money, closing_values[i])
        elif last_SMA30 > last_SMA100 and SMA30 <= SMA100:
            stocks, money = Sell(stocks, money, closing_values[i])
        last_SMA100 = SMA100
        last_SMA30 = SMA30
    if money == 0:
        stocks, money = Sell(stocks, money, closing_values[len(closing_values) - 1])
    return money

# Data loading
file = open('data.csv')
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

# Readings count
N = 1000
# Series save locations
MACD_Save = []
SIGNAL_Save = []
for i in range(N):
    MACD_Save.append(MACD(closing_values, i))
for i in range(N):
    SIGNAL_Save.append(Signal(MACD_Save, i))

x_values = [datetime.strptime(d, "%Y-%m-%d").date() for d in dates]

# Plot showing closing rate in last 1000 days
plt.figure(figsize=(12, 5))
plt.plot(x_values, closing_values)
plt.xlabel('Date of reading')
plt.ylabel('Closing rate')
plt.savefig("closing_rate.png", bbox_inches='tight', dpi=1200)
plt.close()

# MACD + SIGNAL plot
plt.figure(figsize=(12, 5))
plt.plot(x_values, MACD_Save, label="MACD")
plt.plot(x_values, SIGNAL_Save, label="SIGNAL")
for i in range(N - 1):
    if MACD_Save[i] < SIGNAL_Save[i] and MACD_Save[i + 1] >= SIGNAL_Save[i + 1]:
        plt.scatter(x_values[i], MACD_Save[i], color="green", label="Buy", marker='^', alpha=1)
    elif MACD_Save[i] > SIGNAL_Save[i] and MACD_Save[i + 1] <= SIGNAL_Save[i + 1]:
        plt.scatter(x_values[i], MACD_Save[i], color="red", label="Sell", marker='v', alpha=1)
legend_dict = {"MACD": "blue", "Signal": "orange", "Buy": "green", "Sell": "red"}
patchList = []
for key in legend_dict:
    data_key = mpatches.Patch(color=legend_dict[key], label=key)
    patchList.append(data_key)
plt.legend(handles=patchList)
plt.savefig("macd+signal.png", bbox_inches='tight', dpi=1200)

# Transaction - only based on MACD
print("Only MACD + SIGNAL crossings. Beginning balance was 1000. Now you have",
      round(MACD_Investment(1000.00, MACD_Save, SIGNAL_Save, closing_values)))
print("Only MACD + SIGNAL crossings, but with two day delay. Beginning balance was 1000. Now you have",
      round(Delayed_MACD_Investment(1000.00, MACD_Save, SIGNAL_Save, closing_values)))
