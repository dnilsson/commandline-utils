import sys
import urllib.request
import json
import html

max_num_charts = 1000000
year = "1975"
week = "46"

print("Year\tWeek\tPlace\tArtist\tTitle")

for j in range(max_num_charts):
	sys.stderr.write("Scraping year " + year + ", week " + week + ".\n")
	str = urllib.request.urlopen("https://www.sverigetopplistan.se/api/netdata/ghj001.mbr/chart.json?liid=41&dspy=" + year + "&dspp=" + week).read()
	dt = json.loads(str)
	num_items = int(dt["count"])
	if num_items == 0:
		break
	for i in range(num_items):
		item = dt["chart"][i]
		place = item["pbpl"]
		artist = html.unescape(item["arso"])
		song = html.unescape(item["tit"])
		print(year + "\t" + week + "\t" + place + "\t" + artist + "\t" + song)
	nxt = dt["nextper"]
	year = nxt["dspy"]
	week = nxt["dspp"]
