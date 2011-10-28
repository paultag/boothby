
TEMPLATE=./templ
SRC=./src
INC=./include

all:
	@echo "cd into src/"
	@echo ""
	@echo "use \`newobj\` to create a new object"

newobj:
	# Assert $(name) != ""
	$(TEMPLATE)/gen-src $(name) > $(SRC)/$(name).cc
	$(TEMPLATE)/gen-inc $(name) > $(INC)/$(name).hh
