import csv
import re
import pandas as pd
# Regex to parse GAMENAME <space> [ CATEGORY_NAME ] columns (google form output)

exp = r"([a-zA-Z\s0-9<>.]*)\s\[([a-zA-Z]*)\]"

csv_file = pd.read_csv("phase2_scores.csv") 

df = pd.DataFrame(csv_file)

scores = {}
count = {}
avgs = {}

# Ignore the first two columns (timestamp and judge name)
for i, el in enumerate(df.columns[2:]):
    m = re.match(exp, el)
    name = m.group(1)
    cat = m.group(2)
    # Compute means and get the mean for selected category
    avg_cat_score = df.mean(axis=0)[i]

    if name not in scores:
        scores[name] = {}
    # Save average score for category
    scores[name][cat] = avg_cat_score
    if name not in count:
        count[name] = 1
    else:
        count[name] += 1
    if name not in avgs:
        avgs[name] = avg_cat_score
    else:
        avgs[name] += avg_cat_score
    # Compute mean of every category (Overall)
    if count[name] == 5:
        scores[name]["Overall"] = avgs[name] / 5

## Write results to CSV

header = ["Name", "Gameplay", "Technical", "Originality", "Graphics", "Audio", "Overall"]

with open('scores.csv', 'w', encoding='UTF8') as f:
    writer = csv.writer(f)

    # write the header
    writer.writerow(header)

    # write the data
    for i, game in enumerate(scores):
        data = [game]
        for category in header[1:]:
            data.append(scores[game][category])
        writer.writerow(data)

print(scores)
