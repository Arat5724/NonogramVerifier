NAME = nmnm

CXXFLAG = -static-libgcc -static-libstdc++ -O2

INC = include

SRCDIR = src
SRCS = main.cpp Line.cpp Nonogram.cpp NonogramSolver.cpp

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))

all : $(NAME)

bonus : all

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