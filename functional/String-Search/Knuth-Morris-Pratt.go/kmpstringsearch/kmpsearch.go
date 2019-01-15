package kmpstringsearch

// Search pat in content, return the first index of pat, or -1
func Search(content, pat string) int {
	patlen := len(pat)
	conlen := len(content)

	if patlen > conlen || patlen == 0 {
		return -1
	}

	next := make([]int, patlen+1)
	next[0] = 0
	next[1] = 0

	var k int = 0

	for i := 1; i < patlen; i++ {
		k = next[i]
		if k == 0 {
			next[i+1] = 1
		} else if pat[i-1] == pat[k-1] {
			//empty
		} else {
			for k > 0 && pat[i-1] != pat[k-1] {
				k = next[k]
			}
		}
		next[i+1] = k + 1
	}

	for i := 2; i <= patlen; i++ {
		k = next[i]
		if k > 0 && pat[i-1] == pat[k-1] {
			k = next[k]
		}

		next[i] = k
	}

	var j int = 0

	for j < conlen && k < patlen {
		if content[j] != pat[k] {
			if k == 0 {
				k--
			} else {
				n := k + 1
				for n > 0 && content[j] != pat[n-1] {
					n = next[n]
				}
				k = n - 1
			}
		}

		j++
		k++
	}

	if k == patlen {
		return j - patlen
	}
	return -1
}
