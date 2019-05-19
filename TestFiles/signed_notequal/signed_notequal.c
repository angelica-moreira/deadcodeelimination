int main() {
	int i = 0, j = 0, k = 0, l = 0;
	while (i < 100) {
		notrem();
		i++;
	}
	while (k < 100) {
		notrem();
		k++;
	}
	while (j < 100) {
		notrem();
		j++;
	}
	while (l < 100) {
		notrem();
		l++;
	}

	int c = 101;
	while (c < 200) {
		c++;
		notrem();
	}

	/*SLT r1 < r2 -> always true, remove else*/
	if (i < 200) {
		/*should not be removed*/
		notrem();

		/*SGT r1 > r2 -> always true, remove then*/
		if (c > i) {
			/*should not be removed*/
			notrem();
		}
		else {
			/*should be removed*/
			rem();
		}
	}

	else {
		/*should be removed*/
		rem();
	}

	/*SLT r1 > r2 -> always false, remove then*/
	if (c < j) {
		/*should be removed*/
		rem();
	}
	else {
		/*should not be removed*/
		notrem();

		/*SGT r1 < r2 -> always false, remove then*/
		if (k > c) {
			/*should be removed*/
			rem();
		}
		else {
			/*should not be removed*/
			notrem();
		}
	}

	int eq = 333, eq2 = 333, noteq = 334;
	
	/*EQ r1 == r2 -> always true, remove else*/
	if (eq == eq2) {
		/*should not be removed*/
		notrem();

		/*EQ r1 != r2 -> always false, remove then*/
		if (eq == noteq) {
			/*should be removed*/
			rem();
		}
		else {
			/*should not be removed*/
			notrem();
		}
	}
	else {
		/*should be removed*/
		rem();
	}

	/*NEQ r1 == r2 -> always false, remove then*/
	if (eq != eq2) {
		/*should be removed*/
		rem();
	}
	else {
		/*should not be removed*/
		notrem();

		/*NEQ r1 != r2 -> always true, remove else*/
		if (eq != noteq) {
			/*should not be removed*/
			notrem();
		}
		else {
			/*should be removed*/
			rem();
		}
	}

	return 0;
}
