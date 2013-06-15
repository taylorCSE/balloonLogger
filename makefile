# Directories
BUILD_DIR	 = build
DIST	  = dist
SRC_DIR		  = src

# Tools
CPP		  = g++.exe
LINK	  = g++.exe
WINDRES   = windres.exe
RM		= rm -f
ZIP	   = zip

# Output filenames
BIN	   = BalloonLogger.exe

# Metadata
COMMIT = `git rev-parse HEAD`
MAJOR = `cat $(SRC_DIR)/version.h | awk '/MAJOR/{ printf("%d",$$3); }'`
MINOR = `cat $(SRC_DIR)/version.h | awk '/MINOR/{ printf("%d",$$3); }'`
BUILD = `cat $(SRC_DIR)/version.h | awk '/BUILD/{ printf("%d",$$3); }'`
BUILD_INCREMENT = `cat $(SRC_DIR)/version.h | awk '/BUILD/{ printf("%d",$$3+1); }'`

# Source files and objects
SRC_DIRS = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(subst $(SRC_DIR),$(BUILD_DIR),$(patsubst %.cpp,%.o,$(SRC_DIRS)))

# Libraries to include
LIBS	  = --subsystem,windows \
			-mthreads \
			-mwindows \
			-lwx_msw_richtext-2.8 \
			-lwx_msw_aui-2.8 \
			-lwx_msw_xrc-2.8 \
			-lwx_msw_qa-2.8 \
			-lwx_msw_html-2.8 \
			-lwx_msw_adv-2.8 \
			-lwx_msw_core-2.8 \
			-lwx_base_xml-2.8 \
			-lwx_base_net-2.8 \
			-lwx_base-2.8 \
			-lwxregex-2.8 \
			-lwxexpat-2.8 \
			-lwxtiff-2.8 \
			-lwxjpeg-2.8 \
			-lwxpng-2.8 \
			-lkernel32 \
			-luser32 \
			-lgdi32 \
			-lcomdlg32 \
			-lwinmm \
			-lshell32 \
			-lcomctl32 \
			-lole32 \
			-loleaut32 \
			-luuid \
			-lrpcrt4 \
			-ladvapi32 \
			-lodbc32 \
			-lz \
			-lwinspool \
			-lctl3d32 \
			-lwsock32 \
			-llibmysql \
			-lctb

# Compile Flags			
CXXFLAGS  = -fno-exceptions \
			-fno-pcc-struct-return \
			-fstrict-aliasing \
			-ffunction-sections \
			-static-libgcc \
			-static-libstdc++ \
			-Wall \
			-D__WXMSW__ \
			-D__GNUWIN32__ \
			-D__WIN95__ \
			-Wno-deprecated \
			-ggdb \
			-O0

# Color codes

W=\x1b[0m
G=\x1b[32;01m
R=\x1b[31;01m
Y=\x1b[33;01m

.PHONY: all all-before all-after clean clean-custom dist dist-custom todo todo-custom
all: all-before src/version.h $(BIN) all-after

clean: clean-custom
	@echo -e "$(G)Removing temporary files$(W)..."
	-@$(RM) $(OBJ) "$(BIN)"
	@echo -e "$(G)Done$(W)."

# Update version file
	
all-before:
	@echo -e "$(G)Starting Build for $(MAJOR).$(MINOR) $(W)"
	@echo -e "$(G)=======================$(W)"
	@echo -e "Current commit is `git rev-parse HEAD`."
	
	@echo "Updating build number."
	@echo "/* Automatically generated build information */" > $(BUILD_DIR)/_version.h
	@echo "#define VERSION \"$(MAJOR).$(MINOR).$(BUILD_INCREMENT)\"" >> $(BUILD_DIR)/_version.h
	@echo "#define VERSION_COMMIT \"$(COMMIT)\"" >> $(BUILD_DIR)/_version.h
	@echo "#define VERSION_MAJOR $(MAJOR)" >> $(BUILD_DIR)/_version.h
	@echo "#define VERSION_MINOR $(MINOR)" >> $(BUILD_DIR)/_version.h
	@echo "#define VERSION_BUILD $(BUILD_INCREMENT)" >> $(BUILD_DIR)/_version.h
	@rm $(SRC_DIR)/version.h
	@mv $(BUILD_DIR)/_version.h $(SRC_DIR)/version.h
	
	@echo "Writing new VERSION.txt."
	@echo "BalloonGraph $(MAJOR).$(MINOR).$(BUILD)" > VERSION.txt
	@echo "Based on commit $(COMMIT)" >> VERSION.txt
	
	@u2d VERSION.txt

dist: dist-custom
	@echo -e "$(G)Building distribution $(DIST)/balloongraph-$(MAJOR).$(MINOR).$(BUILD).zip$(W)..."
	$(ZIP) $(DIST)/balloongraph-$(MAJOR).$(MINOR).$(BUILD).zip \
			$(BIN) \
			libmysql.dll \
			README.txt \
			CHANGELOG.txt \
			VERSION.txt \
			LICENSE.txt

todo: todo-custom
	-@for file in $(SRC_DIRS); do grep -H -e TODO -e FIXME $$file; done; true
	
# Build exes
	
$(BIN): $(OBJ) $(BUILD_DIR)/icon.o
	@echo -e "Linking $(G)$@...$(W)"
	@$(RM) temp.log temp2.log
	-@$(LINK) $(OBJ) $(BUILD_DIR)/icon.o -o "$(BIN)" $(LIBS) $(CXXFLAGS) 2> temp.log
	@if test -s temp.log; then echo -e "$(R)`cat temp.log`$(W)"; fi;
	@$(RM) temp.log temp2.log

# Build Objects
	
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	@echo -e "Compiling $(G)$<$(W) to $(Y)$@$(W)..."
	@$(RM) temp.log temp2.log
	-@$(CPP) -c $< -o $@ $(CXXFLAGS) 2> temp.log
	@if test -s temp.log; then echo -e "$(R)`cat temp.log`$(W)"; fi;
	@$(RM) temp.log temp2.log
	
$(BUILD_DIR)/icon.o: $(SRC_DIR)/icon.rc
	@echo -e "Compiling icon..."
	@$(RM) temp.log temp2.log
	-@$(WINDRES) "$(SRC_DIR)/icon.rc" "$(BUILD_DIR)/icon.o" 2> temp.log
	@if test -s temp.log; then echo -e "$(R)`cat temp.log`$(W)"; fi;
	@$(RM) temp.log temp2.log
	