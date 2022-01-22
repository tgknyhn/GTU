# Activity Selection Problem
def maxCourse(startTimes, finishTimes):
    size = len(startTimes)

    if size < 1: 
        return 0
    
    lastFinishTimeIndex = 0
    count = 1

    print("Start: ", startTimes[0], "\tFinish:", finishTimes[0])

    for time in range(1, size):
        if (startTimes[time] >= finishTimes[lastFinishTimeIndex]):
            print("Start: ", startTimes[time], "\tFinish:", finishTimes[time])
            lastFinishTimeIndex = time
            count += 1

    return count

startTimes  = [1, 3, 0, 5, 8, 5]
finishTimes = [2, 4, 6, 7, 9, 9]

print("Total Course Count:", maxCourse(startTimes, finishTimes))
