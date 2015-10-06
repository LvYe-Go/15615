BIN=./main
DATA=B-TREE_FILE POSTINGSFILE TEXTFILE
HANDIN=$(USER)-HW3.tar.gz

all: load test

$(BIN):
	cd SRC; make; cp main .. ; cp parms ..

$(DATA):
	\rm -f $(DATA)
	$(BIN) < import_dict.inp > /dev/null

load: $(BIN) $(DATA)

print: load
	$(BIN) < Tests/test_print.inp > print.mysol

test: test_search test_sanity test_leftbracket test_rightbracket
	@echo -e "\033[0;31m" all tests passed "\033[0m"

# make sure your output format is correct
test_sanity: load
	$(BIN) < Tests/test_sanity.inp > sanity.mysol
	@echo -e "\033[0;31m" $@ makes sure you are using identical output format, \
		 so pass me first"\033[0m"
	diff sanity.mysol Tests/sanity.sol
	@echo -e "\033[0;31m" $@ passed "\033[0m"

# for the answers we asked in the handout
test_leftbracket: load
	$(BIN) < Tests/test_leftbracket.inp > leftbracket.mysol
	diff leftbracket.mysol Tests/leftbracket.sol
	@echo -e "\033[0;31m" $@ passed "\033[0m"

# for the answers we asked in the handout
test_rightbracket: load
	$(BIN) < Tests/test_rightbracket.inp > rightbracket.mysol
	diff rightbracket.mysol Tests/rightbracket.sol
	@echo -e "\033[0;31m" $@ passed "\033[0m"

test_search: load
	$(BIN) < Tests/test_search.inp > search.mysol
	diff search.mysol Tests/search.sol
	@echo -e "\033[0;31m" $@ passed "\033[0m"
	
.PHONY: clean grading grade_empty grade_small grade_boundaries spotless handin release solution

clean:
	\rm -f $(BIN)
	cd SRC; make clean
	
solution:
	tar czvhf hw3-solution.tar.gz \
		SRC/get_rightbracket.c \
		SRC/get_leftbracket.c \
		SRC/stats.c \
		SRC/main.c \
		functional_tests.py \
		load_dict.sh \
		import_dict.inp \
		makefile \
		Grading/t?.* \
		Solution.md \
		Tests/leftbracket.sol \
		Tests/rightbracket.sol

grading: $(BIN) grade_empty grade_small grade_boundaries
	@echo "all passed (40 possible points)"
	
grade_empty:
	@rm -f $(DATA) parms
	@cp Grading/t0.parms parms
	@$(BIN) < Grading/t0.inp > t0.mysol
	diff Grading/t0.sol t0.mysol
	@echo "* empty dataset (5 points)"
	
grade_small:
	@rm -f $(DATA) parms
	@cp Grading/t1.parms parms
	@$(BIN) < Grading/t1.inp > t1.mysol
	diff Grading/t1.sol t1.mysol
	@echo "* small dataset (10 points)"
	
grade_boundaries:
	@rm -f $(DATA) parms
	@cp Grading/t2.parms parms
	@tail import_dict.inp | $(BIN) > /dev/null
	@$(BIN) < Grading/t2.inp > t2.mysol
	diff Grading/t2.sol t2.mysol
	@echo "* larger dataset with original params (10 points)"
	
	@rm -f $(DATA) parms
	@cp Grading/t2_alt.parms parms
	@tail import_dict.inp | $(BIN) > /dev/null
	@$(BIN) < Grading/t2.inp > t2_alt.mysol
	diff Grading/t2.sol t2_alt.mysol
	@echo "* larger dataset with different params (15 points)"
	

spotless: clean
	\rm -f $(DATA)
	cd SRC; make spotless
	\rm -f $(BIN) parms
	\rm -f $(HANDIN)
	\rm -f *.mysol

handin: spotless
	tar czvfh $(HANDIN) \
		--exclude=*.o \
		--exclude=*.out \
		--exclude=.svn \
		--exclude=functional_tests.py \
		--exclude=import_dict.inp \
		--exclude=load_dict.sh \
		DOC/ README SRC/ Tests/ makefile Datafiles/

release: spotless
	tar czvfh btree.tar.gz \
		--exclude=*.o \
		--exclude=*.out \
		--exclude=.svn \
		--exclude=functional_tests.py \
		DOC/ README SRC/ Tests/ makefile Datafiles/ import_dict.inp load_dict.sh
