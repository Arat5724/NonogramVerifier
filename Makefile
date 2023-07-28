NAME = nmnm.exe

ifdef DEBUG
CXXFLAG = -static-libgcc -static-libstdc++ -O2 -DDEBUG=1
else
CXXFLAG = -static-libgcc -static-libstdc++ -O2
endif

INC = include

SRCDIR = src

ifdef DEBUG
SRCS = main.cpp Line.cpp Nonogram.cpp NonogramSolver.cpp test.cpp
else
SRCS = main.cpp Line.cpp Nonogram.cpp NonogramSolver.cpp
endif
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))

all : $(NAME)

debug :
	make DEBUG=1

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAG) $(OBJS) -o $(NAME)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAG) -I$(INC) -c $< -o $@

clean :
	rm -rf $(OBJDIR)

fclean : clean
	rm -f $(NAME)

re :
	make fclean
	make all

.PHONY: all bonus clean fclean re