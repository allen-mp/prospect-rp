#include <a_samp>
/*
    The goal is to get x many days shown on a graph
    of the total wealth of a family
*/
#define MAX_DAY_COUNT   10

static Yaxis[] = {
    100000,245000,300000,270000,135000, 160000, 569000, 821000, 749004, 309000
};

static GetMaxYValue(Y[], size_t = sizeof Y) {
    new maximum = 0;
    for(new i = 0; i < size_t; i++) {
        if(Y[i] > maximum) {
            maximum = Y[i];
        }
    }
    return maximum;
}
static GetMinYValue(Y[], size_t = sizeof Y) {
    new minimum = Y[0];
    if(size_t < 1) return minimum;

    for(new i = 1; i < size_t; i++) {
        if(Y[i] < minimum) {
            minimum = Y[i];
        }
    }
    return minimum;
}

//credits RyDeR' (I was lazy...I adjusted it to maintain order)
stock quickSort(array[], order[], left, right)
{
	new
	    tempLeft = left,
	    tempRight = right,
	    pivot = array[(left + right) / 2],
	    tempVar,
        tempOrder
	;
 	while(tempLeft <= tempRight)
	{
	    while(array[tempLeft] < pivot) tempLeft++;
	    while(array[tempRight] > pivot) tempRight--;
	    
	    if(tempLeft <= tempRight)
	    {
	        tempVar = array[tempLeft];
            tempOrder = order[tempLeft];

            array[tempLeft] = array[tempRight];
            order[tempLeft] = order[tempRight];

            array[tempRight] = tempVar;
            order[tempRight] = tempOrder;
	        tempLeft++, tempRight--;
	    }
	}
	if(left < tempRight) quickSort(array, order, left, tempRight);
	if(tempLeft < right) quickSort(array, order, tempLeft, right);
}

/*
    todo: output to textdraw, all data points finished (start at 10 days ago), can add padding around graph of same colour
*/
public OnFilterScriptInit() {
    const screenwidth = 100; //300px = height of graph
    new halfwidth = floatround(screenwidth/2);

    new maximum = GetMaxYValue(Yaxis);
    new minimum = GetMinYValue(Yaxis);
    printf("min: %d, max: %d", minimum, maximum);

    new diff = maximum - minimum;
    new scale = floatround(maximum/screenwidth);
    printf("scale: %d", scale);

    new order[] = {0,1,2,3,4,5,6,7,8,9}; //maintains the order
    quickSort(Yaxis, order, 0, sizeof(Yaxis) - 1);

    //Note since they're ordered -- just place them on Y-axis, we'll move them along X-axis later...
    new cm = minimum;
    new finalYs[MAX_DAY_COUNT+1]; //same array size as Yaxis. finalYs = pixel on screen
    new cursor = 0;
    for(new i; i < sizeof(Yaxis); i++) {
        if(i == 0) {
            printf("(%d,%d)", minimum, Yaxis[i]);
            printf("y: %d, cursor: %d", i, cursor);
            finalYs[i] = cursor;
            continue;
        }

        if(i == sizeof(Yaxis)-1) {
            printf("(%d,%d)", maximum, Yaxis[i]);
            printf("y: %d, cursor: %d", i, 100);
            finalYs[i] = 100;
            break;
        }


        while(1) {
            if(cm > maximum) {
                print("fault: less than or gtr than cm");
                return 1;
            }

            //found in between two data points?
            new cmtmp = cm + scale;
            //printf("cmtmp: %d, cm: %d, Y: %d", cmtmp, cm, Yaxis[i]);
            if(cm < Yaxis[i] < cmtmp) {
                printf("(%d, %d)", cm, Yaxis[i]);
                printf("y: %d, cursor: %d", i, cursor);
                finalYs[i] = cursor;
                break;
            }
            else {
                cm += scale;
                cursor++;
            }
        }
    }

    print("---------------------");
    for(new i; i < MAX_DAY_COUNT; i++) {
        printf("X: %d, Y: %d (old Y: %d)", order[i], finalYs[i], Yaxis[i]);
    }
    return 1;
}

public OnFilterScriptExit() {}

/*
    OUTPUT -- maps Y value to nearest data point in graph
*/
/*

[15:20:19] min: 100000, max: 821000
[15:20:19] scale: 8210
[15:20:19] (100000,100000)
[15:20:19] y: 0, cursor: 0
[15:20:19] (132840, 135000)
[15:20:19] y: 1, cursor: 4
[15:20:19] (157470, 160000)
[15:20:19] y: 2, cursor: 7
[15:20:19] (239570, 245000)
[15:20:19] y: 3, cursor: 17
[15:20:19] (264200, 270000)
[15:20:19] y: 4, cursor: 20
[15:20:19] (297040, 300000)
[15:20:19] y: 5, cursor: 24
[15:20:19] (305250, 309000)
[15:20:19] y: 6, cursor: 25
[15:20:19] (567970, 569000)
[15:20:19] y: 7, cursor: 57
[15:20:19] (748590, 749004)
[15:20:19] y: 8, cursor: 79
[15:20:19] (821000,821000)
[15:20:19] y: 9, cursor: 100
[15:20:19] --------------------- ===== // quicksort back: quicksort(x, y) will reverse
                                    //now all I need to do is add textdraw 'points' at positions (X,Y)
                                    //Oh wait, no need to quicksort back... just textdraw :) order no matter
[15:20:19] X: 0, Y: 0 (old Y: 100000)
[15:20:19] X: 4, Y: 4 (old Y: 135000)
[15:20:19] X: 5, Y: 7 (old Y: 160000)
[15:20:19] X: 1, Y: 17 (old Y: 245000)
[15:20:19] X: 3, Y: 20 (old Y: 270000)
[15:20:19] X: 2, Y: 24 (old Y: 300000)
[15:20:19] X: 9, Y: 25 (old Y: 309000)
[15:20:19] X: 6, Y: 57 (old Y: 569000)
[15:20:19] X: 8, Y: 79 (old Y: 749004)
[15:20:19] X: 7, Y: 100 (old Y: 821000)

*/