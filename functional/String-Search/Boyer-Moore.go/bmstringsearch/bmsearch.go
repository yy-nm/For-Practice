package bmstringsearch

func max(l, r int) int {
	if l > r {
		return l
	} else {
		return r
	}
}

func Search(content, pat string) int {
	patlen := len(pat)
	conlen := len(content)
	if patlen > conlen || patlen == 0 {
		return -1
	}

	// delta1
	delta1 := make([]int, 256)
	for i := 0; i < len(delta1); i++ {
		delta1[i] = patlen
	}

	for i := 0; i < patlen; i++ {
		delta1[pat[i]] = patlen - i - 1
	}
	// delta2
	delta2 := make([]int, patlen)
	for i := 0; i < patlen; i++ {
		delta2[i] = 1
	}

	var movestep int
	var match bool
	for i := 0; i < patlen; i++ {
		movestep = 1
		for movestep < patlen {
			match = true
			for j := patlen - 1; j > i; j-- {
				if movestep <= j && pat[j] != pat[j-movestep] {
					match = false
					break
				}
			}

			if match {
				if i < movestep {
					break
				} else if pat[i] != pat[i-movestep] {
					break
				}
			}
			movestep++
		}

		delta2[i] = movestep + patlen - i - 1
	}

	// start search
	indexc := patlen - 1
	indexp := patlen - 1
	var d1 int
	var d2 int

	for indexc < conlen {

		for indexp >= 0 {
			if pat[indexp] != content[indexc] {
				break
			}

			indexp--
			indexc--
		}

		if indexp < 0 {
			return indexc + 1
		}

		d1 = delta1[content[indexc]]
		d2 = delta2[indexp]

		indexc += max(d1, d2)
		indexp = patlen - 1
	}

	return -1
}
