players = ["G", "S", "H", "M", "D"]


plays = []
for i in range(10):
    for p in players:
        plays.append("{}{}....".format(p, ))