package main

import (
	"binarytree"
	"fmt"
)

func print_tree(t *binarytree.Tree) {
	t.TraversePreorder()
	fmt.Println()
	t.TraverseInorder()
	fmt.Println()
	fmt.Println()
}

func main() {
	var tree binarytree.Tree

	for i := 1; i < 11; i++ {
		tree.AddElement(i)
		print_tree(&tree)
	}

	fmt.Printf("find 1: %d\n", tree.SearchElement(1))
	fmt.Printf("find 11: %d\n", tree.SearchElement(11))

	tree.RemoveElement(4)
	print_tree(&tree)
	tree.RemoveElement(5)
	print_tree(&tree)
	tree.RemoveElement(6)
	print_tree(&tree)
	tree.RemoveElement(7)
	print_tree(&tree)
	tree.RemoveElement(8)
	print_tree(&tree)
	tree.RemoveElement(9)
	print_tree(&tree)
	tree.RemoveElement(2)
	print_tree(&tree)

	tree.FreeTree()

	for i := 1; i < 11; i++ {
		tree.AddElement(11 - i)
		print_tree(&tree)
	}

	fmt.Printf("find 1: %d\n", tree.SearchElement(1))
	fmt.Printf("find 11: %d\n", tree.SearchElement(11))
}
