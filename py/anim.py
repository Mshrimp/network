# _*_ coding:utf-8 _*_

import os
import csv
import datetime
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.dates import DateFormatter
import matplotlib.ticker as ticker

# read the file
filePath = os.path.join(os.getcwd(), "data/anomalyDetect_output.csv")
file = open(filePath, "r")
allData = csv.reader(file)
# skip the first three columns
allData.next()
allData.next()
allData.next()
# cache the data
data = [line for line in allData]
# for i in data: print i

# take out the target value
timestamp = [line[0] for line in data]
value = [line[1:] for line in data]


# format the time style 2016-12-01 00:00:00
def timestampFormat(t):
	result = datetime.datetime.strptime(t, "%Y-%m-%d %H:%M:%S")
	return result


# take out the data
timestamp = map(timestampFormat, timestamp)
value_a = [float(x[0]) for x in value]
predict_a = [float(x[1]) for x in value]
anomalyScore_a = [float(x[2]) for x in value]

# initial the size of the figure
fig = plt.figure(figsize=(18, 8), facecolor="white")
fig.subplots_adjust(left=0.06, right=0.70)
ax1 = fig.add_subplot(2, 1, 1)
ax2 = fig.add_subplot(2, 1, 2)
ax3 = fig.add_axes([0.8, 0.1, 0.2, 0.8], frameon=False)

# initial plot
p1, = ax1.plot_date([], [], fmt="-", color="red", label="actual")
ax1.legend(loc="upper right", frameon=False)
ax1.grid(True)
p2, = ax2.plot_date([], [], fmt="-", color="red", label="anomaly score")
ax2.legend(loc="upper right", frameon=False)
ax2.axhline(0.8, color='black', lw=2)
# add the x/y label
ax2.set_xlabel("date time")
ax2.set_ylabel("anomaly score")
ax1.set_ylabel("value")
# add the table in ax3
col_labels = ["date time", 'actual value', 'predict value', 'anomaly score']
ax3.text(0.05, 0.99, "anomaly value table", size=12)
ax3.set_xticks([])
ax3.set_yticks([])

# axis format
dateFormat = DateFormatter("%m/%d %H:%M")
ax1.xaxis.set_major_formatter(ticker.FuncFormatter(dateFormat))
ax2.xaxis.set_major_formatter(ticker.FuncFormatter(dateFormat))


# define the initial function
def init():
	p1.set_data([], [])
	p2.set_data([], [])
   return p1, p2


# initial data for the update function
x1 = []
x2 = []
x1_2 = []
y1_2 = []
x1_3 = []
y1_3 = []
y1 = []
y2 = []
highlightList = []
turnOn = True
tableValue = [[0, 0, 0, 0]]


# update function
def stream(i):
   # update the main graph(contains actual value and predicted value)
   # add the data
   global turnOn, highlightList, ax3

   x1.append(timestamp[i])
	y1.append(value_a[i])
# update the axis
	minAxis = max(x1) - datetime.timedelta(days=1)
	ax1.set_xlim(minAxis, max(x1))
	ax1.set_ylim(min(y1), max(y1))
	ax1.figure.canvas.draw()
	p1.set_data(x1, y1)

# update the anomaly graph(contains anomaly score)
	x2.append(timestamp[i])
	y2.append(anomalyScore_a[i])
	ax2.set_xlim(minAxis, max(x2))
	ax2.set_ylim(min(y2), max(y2))

# update the scatter
	if anomalyScore_a[i] >= 0.8:
		x1_3.append(timestamp[i])
		y1_3.append(value_a[i])
		ax1.scatter(x1_3, y1_3, s=50, color="black")

				# update the high light
	if anomalyScore_a[i] >= 0.8:
		highlightList.append(i)
		turnOn = True
	else:
		turnOn = False
	if len(highlightList) != 0 and turnOn is False:

		ax2.axvspan(timestamp[min(highlightList)] - datetime.timedelta(minutes=10),
					timestamp[max(highlightList)] + datetime.timedelta(minutes=10),
					color='r',
					edgecolor=None,
					alpha=0.2)
		highlightList = []
		turnOn = True
	p2.set_data(x2, y2)

	    # add the table in ax3
	    # update the anomaly tabel
	if anomalyScore_a[i] >= 0.8:
		ax3.remove()
		ax3 = fig.add_axes([0.8, 0.1, 0.2, 0.8], frameon=False)
		ax3.text(0.05, 0.99, "anomaly value table", size=12)
		ax3.set_xticks([])
		ax3.set_yticks([])
		tableValue.append([timestamp[i].strftime("%Y-%m-%d %H:%M:%S"), value_a[i], predict_a[i], anomalyScore_a[i]])
		if len(tableValue) >= 40: tableValue.pop(0)
		ax3.table(cellText=tableValue, colWidths=[0.35] * 4, colLabels=col_labels, loc=1, cellLoc="center")

	return p1, p2


# main animated function
anim = FuncAnimation(fig, stream, init_func=init, frames=len(timestamp), interval=0)

plt.show()
file.close()
