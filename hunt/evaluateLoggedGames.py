import os

numGames = 0
roundsTotal = 0
scoresTotal = 0
draculaWin = 0

for f in os.listdir("logs"):
    log = open(os.path.join("logs", f), "r")
    lines = log.readlines()
    roundNum = int(lines[-1].split(":")[-1].strip())
    score = int(lines[-2].split(":")[-1].strip())
    print(f)
    print("Round: ", roundNum)
    print("Score: ", score)
    print()
    roundsTotal += roundNum
    scoresTotal += score
    numGames += 1
    if score <= 0:
        draculaWin += 1

print("Overall Results: ")
print("Dracula Won: {} games".format(draculaWin))
print("Hunters Won: {} games".format(numGames - draculaWin))
print()
print("Average rounds in games: {}".format(roundsTotal / numGames))
print("Average score in games: {}".format(scoresTotal / numGames))