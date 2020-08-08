import os
import statistics

numGames = 0
roundsTotal = 0
scoresTotal = 0
draculaWin = 0
scores = []
for f in os.listdir("logs"):
    log = open(os.path.join("logs", f), "r")
    content = log.read()
    lines = content.split("\n")
    print(f)
    try:
        roundNum = int(lines[-2].split(":")[-1].strip())
        score = int(lines[-3].split(":")[-1].strip())
        if "disq" in content:
            print("Disqualified")
        print("Round: ", roundNum)
        print("Score: ", score)
        print()
        roundsTotal += roundNum
        scoresTotal += score
        scores.append(score)
        numGames += 1
        if score <= 0:
            draculaWin += 1
    except Exception as e:
        print("Error")
        print(e)
        continue

print("Overall Results: ")
print("Dracula Won: {} games".format(draculaWin))
print("Hunters Won: {} games".format(numGames - draculaWin))
print()
print("Average rounds in games: {}".format(roundsTotal / numGames))
print("Average score in games: {}".format(scoresTotal / numGames))
print("Standard Score Deviation: {}".format(statistics.stdev(scores)))
