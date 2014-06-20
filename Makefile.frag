.PHONY: coverage

DATE=`date +%Y-%m-%d--%H-%M-%S`
coverage:
	@if test -e $(top_srcdir)/coverage; then \
		echo "Moving previous coverage run to $(DATE)"; \
		mv coverage coverage-$(DATE); \
	fi
	lcov --gcov-tool $(top_srcdir)/.llvm-cov.sh --capture --derive-func-data --directory . --output-file .coverage.lcov --no-external
	genhtml .coverage.lcov --legend --title "phongo code coverage" --output-directory coverage


