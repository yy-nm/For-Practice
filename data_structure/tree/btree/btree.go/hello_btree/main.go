package main

import (
	"btree"
	"fmt"
)

func main() {
	var tree btree.BTree23

	for i := 1; i < 11; i++ {
		tree.AddElement(i)
		tree.TraverseTree()
	}

	fmt.Printf("find 1: %d\n", tree.SearchElement(1))
	fmt.Printf("find 11: %d\n", tree.SearchElement(11))

	tree.DeleteElement(4)
	tree.TraverseTree()
	tree.DeleteElement(5)
	tree.TraverseTree()
	tree.DeleteElement(6)
	tree.TraverseTree()
	tree.DeleteElement(7)
	tree.TraverseTree()
	tree.DeleteElement(8)
	tree.TraverseTree()
	tree.DeleteElement(9)
	tree.TraverseTree()
	tree.DeleteElement(2)
	tree.TraverseTree()

	tree.FreeTree()

	for i := 1; i < 11; i++ {
		tree.AddElement(11 - i)
		tree.TraverseTree()
	}

	fmt.Printf("find 1: %d\n", tree.SearchElement(1))
	fmt.Printf("find 11: %d\n", tree.SearchElement(11))
}
