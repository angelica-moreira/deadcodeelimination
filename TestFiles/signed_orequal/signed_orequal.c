int main() {
	int i = 0, j = 0, k = 0, l = 0;
	while (i < 100) {
		i++;
		notrem();
	}
	
	while (j < 100) {
		j++;
		notrem();
	}
	
	while (k < 100) {
		k++;
		notrem();
	}
	
	while (l < 100) {
		l++;
		notrem();
	}

	int c = 101;
	while (c < 200) {
		c++;
		notrem();
	}

	/*SLE r1 < r2 -> always true, remove else*/
	if (i <= 200) {
		/*should not be removed*/
		notrem();

		/*SGE r1 > r2 -> always true, remove else*/
		if (c >= i) {
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

	/*SLE r1 > r2 -> always false, remove then*/
	if (c <= j) {
		/*should be removed*/
		rem();
	}
	else {
		/*should not be removed*/
		notrem();

		/*SGE r1 < r2 -> always false, remove then*/
		if (k >= c) {
			/*should be removed*/
			rem();
		}
		else {
			/*should not be removed*/
			notrem();
		}
	}

	return 0;
}
