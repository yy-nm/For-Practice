package bplustree

import "fmt"

const (
	b = 7
)

type node struct {
	isleaf   bool
	keys     [b]int
	keycount int
	childs   [b + 1]*node
}

type Tree struct {
	root *node
}

func GetEmptyBTree234() *Tree {
	return &Tree{
		root: &node{
			isleaf:   true,
			keycount: 0,
		},
	}
}

func searchNode(n *node, k int) bool {
	for !n.isleaf {
		index := n.keycount - 1
		for index >= 0 {
			if n.keys[index] <= k {
				break
			}
			index--
		}

		if index < 0 {
			return false
		}
		n = n.childs[index]
	}

	for i := 0; i < n.keycount; i++ {
		if k == n.keys[i] {
			return true
		}
	}

	return false
}

func modifyNodeKeys(n *node, k int, parents []*node) {
	index := n.keycount - 1
	for index >= 0 {
		if n.keys[index] < k {
			break
		}
		n.keys[index+1] = n.keys[index]
		index--
	}

	oldk := n.keys[0]
	index++
	n.keys[index] = k
	n.keycount++

	if index == 0 {
		goon := false
		for len(parents) > 0 {
			p := parents[len(parents)-1]
			parents = parents[:len(parents)-1]
			goon = false

			for i := 0; i < p.keycount; i++ {
				if p.keys[i] == oldk {
					p.keys[i] = k
					goon = i == 0
					break
				}
			}

			if !goon {
				break
			}
		}
	}
}

func modifyParentKeys(oldk, newk int, parents []*node) {
	for len(parents) > 0 {
		p := parents[len(parents)-1]
		parents = parents[:len(parents)-1]
		oldpk := p.keys[0]
		goon := false
		for i := 0; i < p.keycount; i++ {
			if p.keys[i] == oldk {
				p.keys[i] = newk
				goon = i == 0
				break
			}
		}

		if !goon {
			break
		}

		oldk = oldpk
	}
}

func insertInternalNode(t *Tree, k int, l, r *node, parents []*node) {
	if 0 == len(parents) {
		root := &node{}
		root.isleaf = false
		t.root = root

		root.childs[0] = l
		root.childs[1] = r

		root.keys[0] = l.keys[0]
		root.keys[1] = r.keys[0]

		root.keycount = 2
	} else {
		p := parents[len(parents)-1]
		parents = parents[:len(parents)-1]
		index := -1
		for i := 0; i < p.keycount; i++ {
			if p.keys[i] == k {
				index = i
				break
			}
		}

		if index < 0 {
			index = -1
		}

		if p.keycount < b {
			oldpk := p.keys[0]
			for i := p.keycount - 1; i > index; i-- {
				p.keys[i+1] = p.keys[i]
				p.childs[i+1] = p.childs[i]
			}
			p.childs[index+1] = r
			p.keys[index+1] = r.keys[0]
			p.keys[index] = l.keys[0]
			p.keycount++
			if index == 0 && p.keys[0] != oldpk {
				modifyParentKeys(oldpk, p.keys[0], parents)
			}
		} else {
			rk := r.keys[0]
			pk := p.keys[0]

			pl := p
			pr := &node{}
			pr.isleaf = pl.isleaf
			middleindex := (b+1)/2 + (b+1)%2
			isleft := (index + 1) < middleindex
			count := (b + 1) / 2

			if isleft {
				middleindex--
			} else {
				count--
			}

			for i := 0; i < count; i++ {
				pr.keys[i] = pl.keys[i+middleindex]
				pr.childs[i] = pl.childs[i+middleindex]
			}
			pr.keycount = count
			pl.keycount -= count

			insertnode := pr
			if isleft {
				insertnode = pl
			}

			pindex := insertnode.keycount - 1
			for pindex >= 0 {
				if insertnode.keys[pindex] < rk {
					break
				}
				insertnode.keys[pindex+1] = insertnode.keys[pindex]
				insertnode.childs[pindex+1] = insertnode.childs[pindex]
				pindex--
			}
			insertnode.keys[pindex+1] = rk
			insertnode.childs[pindex+1] = r
			insertnode.keycount++

			if index < pl.keycount {
				pl.keys[index] = l.keys[0]
			} else {
				pr.keys[index-pl.keycount] = l.keys[0]
			}

			insertInternalNode(t, pk, pl, pr, parents)
		}
	}
}

func splitLeafChild(t *Tree, n *node, k int, parents []*node) {
	oldk := n.keys[0]

	l := n
	r := &node{}
	r.isleaf = l.isleaf

	middleindex := (b+1)/2 + (b+1)%2
	isleft := n.keys[middleindex-1] > k
	count := (b + 1) / 2

	if isleft {
		middleindex--
	} else {
		count--
	}

	for i := 0; i < count; i++ {
		r.keys[i] = l.keys[i+middleindex]
	}
	r.keycount = count
	l.keycount -= count

	insertnode := r
	if isleft {
		insertnode = l
	}

	index := insertnode.keycount - 1
	for index >= 0 {
		if insertnode.keys[index] < k {
			break
		}
		insertnode.keys[index+1] = insertnode.keys[index]
		index--
	}

	insertnode.keys[index+1] = k
	insertnode.keycount++

	r.childs[b] = l.childs[b]
	l.childs[b] = r

	insertInternalNode(t, oldk, l, r, parents)
}

func insertNode(t *Tree, n *node, k int) {
	parents := make([]*node, 0)

	for !n.isleaf {
		parents = append(parents, n)
		index := n.keycount - 1
		for index >= 0 {
			if n.keys[index] < k {
				break
			}
			index--
		}

		if index < 0 {
			index = 0
		}

		n = n.childs[index]
	}

	if n.keycount == b {
		splitLeafChild(t, n, k, parents)
	} else {
		modifyNodeKeys(n, k, parents)
	}
}

func mergeNode(t *Tree, n *node, k int, parents []*node) {
	p := parents[len(parents)-1]
	parents = parents[:len(parents)-1]

	oldpk := p.keys[0]
	pindex := -1
	for i := 0; i < p.keycount; i++ {
		if k == p.keys[i] {
			pindex = i
			break
		}
	}

	l := p.childs[pindex]
	r := p.childs[pindex+1]

	lcount := l.keycount
	for i := 0; i < r.keycount; i++ {
		l.keys[i+lcount] = r.keys[i]
		if !l.isleaf {
			l.childs[i+lcount] = r.childs[i]
		}
	}

	if l.isleaf {
		l.childs[b] = r.childs[b]
	}

	l.keycount += r.keycount
	r.keycount = 0

	for i := pindex + 1; i < p.keycount-1; i++ {
		p.keys[i] = p.keys[i+1]
		p.childs[i] = p.childs[i+1]
	}
	p.keycount--
	p.keys[0] = p.childs[0].keys[0]

	mincount := (b + 1) / 2
	if len(parents) == 0 {
		if p.keycount == 1 {
			t.root = p.childs[0]
			p.keycount = 0
		}
	} else if p.keycount < mincount {
		keepNodeKeyCountCorrect(t, p, oldpk, parents)
	} else if oldpk != p.keys[0] {
		modifyParentKeys(oldpk, p.keys[0], parents)
	}
}

func keepNodeKeyCountCorrect(t *Tree, n *node, oldk int, parents []*node) {

	p := parents[len(parents)-1]
	pindex := -1
	for i := 0; i < p.keycount; i++ {
		if oldk == p.keys[i] {
			pindex = i
			break
		}
	}

	mincount := (b + 1) / 2
	if pindex < p.keycount-1 && p.childs[pindex+1].keycount > mincount {
		r := p.childs[pindex+1]
		n.keys[n.keycount] = r.keys[0]
		if !n.isleaf {
			n.childs[n.keycount] = r.childs[0]
		}
		n.keycount++

		oldrk := r.keys[0]
		for i := 1; i < r.keycount; i++ {
			r.keys[i-1] = r.keys[i]
			if !r.isleaf {
				r.childs[i-1] = r.childs[i]
			}
		}

		r.keycount--

		modifyParentKeys(oldrk, r.keys[0], parents)
		if oldk != n.keys[0] {
			modifyParentKeys(oldk, n.keys[0], parents)
		}
	} else if pindex > 0 && p.childs[pindex-1].keycount > mincount {
		l := p.childs[pindex-1]

		for i := n.keycount - 1; i >= 0; i-- {
			n.keys[i+1] = n.keys[i]
			if !n.isleaf {
				n.childs[i+1] = n.childs[i]
			}
		}

		n.keys[0] = l.keys[l.keycount-1]
		if !n.isleaf {
			n.childs[0] = l.childs[l.keycount-1]
		}
		n.keycount++
		l.keycount--

		modifyParentKeys(oldk, n.keys[0], parents)
	} else {
		if pindex < p.keycount-1 {
			mergeNode(t, n, oldk, parents)
		} else if pindex > 0 {
			mergeNode(t, p.childs[pindex-1], p.childs[pindex-1].keys[0], parents)
		}
	}
}

func deleteNode(t *Tree, n *node, k int) {
	parents := make([]*node, 0)

	for !n.isleaf {
		parents = append(parents, n)

		for i := n.keycount - 1; i >= 0; i-- {
			if n.keys[i] <= k {
				n = n.childs[i]
				break
			}
		}
	}

	mincount := (b + 1) / 2
	index := -1
	for i := 0; i < n.keycount; i++ {
		if n.keys[i] == k {
			index = i
			break
		}
	}

	oldk := n.keys[0]
	for i := index; i < n.keycount-1; i++ {
		n.keys[i] = n.keys[i+1]
	}
	n.keycount--

	if n.keycount >= mincount || len(parents) == 0 {
		if oldk != n.keys[0] {
			modifyParentKeys(oldk, n.keys[0], parents)
		}
	} else {
		keepNodeKeyCountCorrect(t, n, oldk, parents)
	}
}

func (t *Tree) Search(key int) bool {
	if nil == t || nil == t.root {
		return false
	}

	return searchNode(t.root, key)
}

func (t *Tree) Insert(key int) {
	if t == nil {
		return
	}

	if t.Search(key) {
		return
	}

	if nil == t.root {
		t.root = &node{
			isleaf: true,
		}
	}

	insertNode(t, t.root, key)
}

func (t *Tree) Delete(key int) {
	if t == nil {
		return
	}

	if !t.Search(key) {
		return
	}

	deleteNode(t, t.root, key)
}

func (t *Tree) FreeTree() {
	if nil != t {
		t.root = nil
	}
}

func (t *Tree) TraverseTree() {
	fmt.Println()

	if nil == t || nil == t.root {
		return
	}

	q := make([]*node, 0)
	q = append(q, t.root)
	q = append(q, nil)

	for len(q) > 0 {
		n := q[0]
		q = q[1:]
		if nil == n {
			fmt.Println()
			if len(q) == 0 {
				break
			}
			q = append(q, nil)
		} else {
			fmt.Print("    ")
			for i := 0; i < n.keycount; i++ {
				fmt.Print(n.keys[i])
				fmt.Print(" ")
				if !n.isleaf {
					q = append(q, n.childs[i])
				}
			}
		}
	}
}

func (t *Tree) TraverseData() {
	if nil == t || nil == t.root {
		return
	}

	n := t.root

	for !n.isleaf {
		n = n.childs[0]
	}

	for nil != n {
		for i := 0; i < n.keycount; i++ {
			fmt.Print(n.keys[i])
			fmt.Print(" ")
		}
		fmt.Print("    ")
		n = n.childs[b]
	}
}
