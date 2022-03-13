# Tools for csv manipulation
import csv
# Tools for date strings manipulation
from datetime import datetime
# Tools for
import matplotlib.pyplot as plt


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
    money_after = 0
    return balance_after, money_after


# Function that sells stocks. I consider that the number of stocks sold can only be an integer
def Sell(balance_before, money_after, exchangerate):
    balance_after = 0
    money_after += balance_before * exchangerate
    return balance_after, money_after


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
plt.plot(x_values, MACD_Save)
plt.plot(x_values, SIGNAL_Save)
plt.savefig("macd+signal.png", bbox_inches='tight', dpi=1200)

# Adding closing rate
plt.figure(figsize=(12, 5))
plt.plot(x_values, closing_values)
# Vertical lines showing where SIGNAL and MACD have a cross-point
for i in range(N - 1):
    if (MACD_Save[i] < SIGNAL_Save[i] and MACD_Save[i + 1] > SIGNAL_Save[i + 1]) or (
            MACD_Save[i] > SIGNAL_Save[i] and MACD_Save[i + 1] < SIGNAL_Save[i + 1]):
        plt.axvline(x_values[i], color="orange")
plt.savefig("macd+signal_closing_rate.png", bbox_inches='tight', dpi=1200)
plt.close()

# Transaction - only based on MACD
balance = 1000.0
money = 0.0
for i in range(N - 1):
    # When it's time to buy - the exchange rate will rise
    if MACD_Save[i] < SIGNAL_Save[i] and MACD_Save[i + 1] > SIGNAL_Save[i + 1] and balance == 0:
        balance, money = Buy(balance, money, closing_values[i])
    elif MACD_Save[i] > SIGNAL_Save[i] and MACD_Save[i + 1] < SIGNAL_Save[i + 1] and balance != 0:
        balance, money = Sell(balance, money, closing_values[i])
if balance == 0:
    balance, money = Buy(balance, money, closing_values[len(closing_values) - 1])
print("Only MACD + SIGNAL crossings. Beginning balance was 1000. Now you have", balance)
