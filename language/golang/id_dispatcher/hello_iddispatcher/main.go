package main

import "iddispatcher"
import "fmt"

func getids(id iddispatcher.Dispatcher, array []int, count int, finishcount chan int) {
	for i := 0; i < count; i++ {
		array[i] = id.GetID()
	}

	finishcount <- 1
}

func main() {

	id := iddispatcher.GetADispather(100000000)

	concurrencycount := 5
	allocidcount := 100000
	arrays := make([][]int, concurrencycount)
	finishcount := make(chan int, concurrencycount)

	for i := 0; i < concurrencycount; i++ {
		arrays[i] = make([]int, allocidcount)
	}

	for i := 0; i < concurrencycount; i++ {
		go getids(id, arrays[i], allocidcount, finishcount)
	}

	for len(finishcount) != concurrencycount {
	}

	countmap := make(map[int]bool)
	//var _, ok bool

	for i := 0; i < concurrencycount; i++ {
		for j := 0; j < allocidcount; j++ {
			_, ok := countmap[arrays[i][j]]
			if ok {
				panic(fmt.Sprintf("has same id %v", arrays[i][j]))
			}
		}
	}
}
