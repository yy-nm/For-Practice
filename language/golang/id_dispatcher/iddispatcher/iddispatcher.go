package iddispatcher

import "fmt"

type Dispatch struct {
	id         chan int
	duringLoad chan int
	maxValue   int
	nowID      int
}

const idDefaultCount = 10
const idDefaultLoadCount = idDefaultCount

const idDefaultValue = 0

func GetADispather(maxvalue int) Dispatcher {
	if maxvalue < 1 {
		panic(fmt.Sprintf("error maxvalue: %v", maxvalue))
	}

	return &Dispatch{
		id:         make(chan int, idDefaultCount),
		duringLoad: make(chan int, 1),
		maxValue:   maxvalue,
		nowID:      idDefaultValue,
	}
}

func reloadIDs(dispatch *Dispatch) {
	if len(dispatch.id) > 0 {
		return
	}

	dispatch.duringLoad <- 1

	if len(dispatch.id) > 0 {
		<-dispatch.duringLoad
		return
	}

	for i := 1; i <= idDefaultLoadCount; i++ {
		if dispatch.nowID > dispatch.maxValue {
			dispatch.nowID = idDefaultValue + 1
		} else {
			dispatch.nowID++
		}

		select {
		case dispatch.id <- dispatch.nowID:
		default:
			<-dispatch.duringLoad
			return
		}
	}
	<-dispatch.duringLoad
}

func (patcher *Dispatch) GetID() int {
	if nil == patcher {
		return -1
	}

	for {
		select {
		case id := <-patcher.id:
			return id
		default:
			reloadIDs(patcher)
		}
	}
}
