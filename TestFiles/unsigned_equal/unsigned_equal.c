int main() {
	unsigned int i = 0U, j = 0U, k = 0U, l = 0U;
	while (i < 100U) {
		i++;
		notrem();
	}
	
	while (j < 100U) {
		j++;
		notrem();
	}
	
	while (k < 100U) {
		k++;
		notrem();
	}
	
	while (l < 100U) {
		l++;
		notrem();
	}

	unsigned int c = 101U;
	while (c < 200U) {
		c++;
		notrem();
	}

	/*ULE r1 < r2 -> always true, remove else*/
	if (i <= 200U) {
		/*should not be removed*/
		notrem();

		/*UGE r1 > r2 -> always true, remove else*/
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

	/*ULE r1 > r2 -> always false, remove then*/
	if (c <= j) {
		/*should be removed*/
		rem();
	}
	else {
		/*should not be removed*/
		notrem();

		/*UGE r1 < r2 -> always false, remove then*/
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
