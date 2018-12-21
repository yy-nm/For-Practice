package main

import (
	"bplustree"
	"fmt"
)

func main() {
	var tree bplustree.Tree

	for i := 1; i < 111; i++ {
		tree.Insert(i)
		tree.TraverseTree()
	}

	fmt.Printf("find 1: %d\n", tree.Search(1))
	fmt.Printf("find 11: %d\n", tree.Search(11))

	tree.Delete(4)
	tree.TraverseTree()
	tree.Delete(5)
	tree.TraverseTree()
	tree.Delete(6)
	tree.TraverseTree()
	tree.Delete(7)
	tree.TraverseTree()
	tree.Delete(8)
	tree.TraverseTree()
	tree.Delete(9)
	tree.TraverseTree()
	tree.Delete(2)
	tree.TraverseTree()

	tree.Delete(13)
	tree.TraverseTree()
	tree.Delete(1)
	tree.TraverseTree()

	tree.Delete(106)
	tree.TraverseTree()
	tree.Delete(107)
	tree.TraverseTree()
	tree.Delete(108)
	tree.TraverseTree()

	tree.Insert(8)
	tree.TraverseTree()

	fmt.Println()
	fmt.Println()
	tree.TraverseData()
	fmt.Println()

	tree.FreeTree()

	for i := 1; i < 11; i++ {
		tree.Insert(11 - i)
		tree.TraverseTree()
	}

	fmt.Printf("find 1: %d\n", tree.Search(1))
	fmt.Printf("find 11: %d\n", tree.Search(11))
}
