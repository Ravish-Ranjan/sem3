import random
from pprint import pprint

TOTAL_REQUESTS = 100
FALIURE_RATE = 0.7 

def stop_and_wait():
    total = 0
    success = 0
    failiure = 0

    for i in range(TOTAL_REQUESTS):
        total+=1
        if (random.random() >= FALIURE_RATE):
            success+=1
        else :
            failiure+=1

    return {
        "total":total,
        "success":success,
        "failiure":failiure,
        "failiure_rate":failiure/total,
        "re-attempts":0 
    }

def stop_and_wait_arq():
    total = 0
    success = 0
    failiure = 0
    re_attempts = 0

    for i in range(TOTAL_REQUESTS):
        while (True):
            total+=1
            if (random.random() >= FALIURE_RATE):
                success+=1
                break
            else :
                re_attempts+=1
                failiure+=1

    return {
        "total":total,
        "success":success,
        "failiure":failiure,
        "failiure_rate":failiure/total,
        "re-attempts":re_attempts
    }


pprint(stop_and_wait())
pprint(stop_and_wait_arq())