package trie

import (
	"fmt"
)

type childnode struct {
	value byte
	child *node
	next  *childnode
}

type node struct {
	children      *childnode
	childcount    int
	childvaluemap [8]int
	isend         bool
}

type Trie struct {
	root *node
}

var bitwiseMap = []int{
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
}

const kOffset = 5
const kMask = (1 << kOffset) - 1

func getChildNode() *childnode {
	return &childnode{
		value: 0,
		child: nil,
		next:  nil,
	}
}

func getNode() *node {
	return &node{
		children:      nil,
		childcount:    0,
		childvaluemap: [8]int{0, 0, 0, 0, 0, 0, 0, 0},
		isend:         false,
	}
}

// GetTrie get a Trie variable
func GetTrie() *Trie {
	return &Trie{
		root: nil,
	}
}

func (t *Trie) InsertString(s string) {
	if t == nil {
		return
	}

	t.InsertSlice([]byte(s))
}

func (t *Trie) InsertSlice(s []byte) {
	if t == nil {
		return
	}

	if t.root == nil {
		t.root = getNode()
	}

	n := t.root
	var b byte
	var child *childnode
	for i := 0; i < len(s); i++ {
		b = s[i]
		if (n.childvaluemap[b>>kOffset] & bitwiseMap[b&kMask]) == 0 {
			n.childvaluemap[b>>kOffset] |= bitwiseMap[b&kMask]
			n.childcount++

			child = getChildNode()
			child.child = getNode()
			child.value = b
			child.next = n.children
			n.children = child
		} else {
			child = n.children
			for child != nil && child.value != b {
				child = child.next
			}
		}

		n = child.child
	}

	n.isend = true
}

func (t *Trie) SearchString(s string) bool {
	if t == nil {
		return false
	}

	return t.SearchSlice([]byte(s))
}

func (t *Trie) SearchSlice(s []byte) bool {
	if t == nil || t.root == nil {
		return false
	}

	n := t.root
	var b byte
	var child *childnode
	for i := 0; i < len(s); i++ {
		b = s[i]
		if (n.childvaluemap[b>>kOffset] & bitwiseMap[b&kMask]) == 0 {
			return false
		}

		child = n.children
		for child != nil && child.value != b {
			child = child.next
		}

		if child == nil {
			return false
		}

		n = child.child
	}

	return n != nil && n.isend
}

func (t *Trie) DeleteString(s string) {
	if t == nil {
		return
	}

	t.DeleteSlice([]byte(s))
}

func (t *Trie) DeleteSlice(s []byte) {
	if t == nil {
		return
	}

	if !t.SearchSlice(s) {
		return
	}

	parents := make([]*node, 0)
	chars := make([]byte, 0)
	n := t.root
	var b byte
	var child *childnode
	for i := 0; i < len(s); i++ {
		b = s[i]
		parents = append(parents, n)
		chars = append(chars, b)
		child = n.children
		for child != nil && child.value != b {
			child = child.next
		}

		n = child.child
	}

	if n.childcount > 1 {
		n.isend = false
		return
	}

	var p *node
	for len(parents) > 0 {
		p = parents[len(parents)-1]
		parents = parents[:len(parents)-1]
		b = chars[len(chars)-1]
		chars = chars[:len(chars)-1]
		if p.isend || p.childcount > 1 {
			break
		}
	}

	p.childvaluemap[b>>kOffset] &= ^bitwiseMap[b&kMask]
	p.childcount--
	child = p.children
	var childpre *childnode
	for child != nil && child.value != b {
		childpre = child
		child = child.next
	}

	if childpre == nil {
		p.children = p.children.next
	} else {
		childpre.next = child.next
	}
}

func printString(n *node, s []byte) {
	if n.isend {
		fmt.Println(string(s))
	}

	child := n.children
	for child != nil {
		s = append(s, child.value)
		printString(child.child, s)
		s = s[:len(s)-1]

		child = child.next
	}
}

func (t *Trie) PrintAllString() {
	if t == nil || t.root == nil {
		return
	}

	q := make([]byte, 0)
	printString(t.root, q)
}
