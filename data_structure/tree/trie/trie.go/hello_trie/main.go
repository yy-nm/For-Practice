package main

import (
	"fmt"
	"trie"
)

func main() {

	t := trie.GetTrie()

	insertstr := []string{
		"hello world",
		"hello world1",
		"helloworld",
		"hello",
	}

	searchstr := []string{
		"hello world",
		"helo world",
		"kello",
		"helloworld ",
	}

	deletestr := []string{
		"hello ",
		"hello world",
		"hello ",
		"hello world1",
		"hello ",
	}

	for i := 0; i < len(insertstr); i++ {
		t.InsertString(insertstr[i])
	}

	for i := 0; i < len(searchstr); i++ {
		fmt.Println(searchstr[i], t.SearchString(searchstr[i]))
	}

	for i := 0; i < len(deletestr); i++ {
		t.DeleteString(deletestr[i])
	}

	t.PrintAllString()
}
