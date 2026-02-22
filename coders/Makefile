NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
INCLUDES    = -I include
HEADER      = include/codexion.h

SRC_DIR     = src/

DATA_DIR    = $(SRC_DIR)data/
UTILS_DIR   = $(SRC_DIR)utils/
PARSE_DIR   = $(SRC_DIR)parse/
SIM_DIR     = $(SRC_DIR)simulation/

PARSE_FILES = parse.c
DATA_FILES  = dongle_create.c data_create.c coders_create.c
UTILS_FILES = ft_strcmp.c ft_atol.c dongle_utils.c write_status.c \
			  checking_burn.c utils_routine.c
CORE_FILES  = virtual.c monitoring.c fifo_compile.c \
			  routine.c edf_compile.c solo_coder.c

SRC = $(addprefix $(DATA_DIR), $(DATA_FILES)) \
      $(addprefix $(UTILS_DIR), $(UTILS_FILES)) \
      $(addprefix $(PARSE_DIR), $(PARSE_FILES)) \
      $(addprefix $(SIM_DIR), $(CORE_FILES))\
      main.c

OBJS = $(SRC:.c=.o)
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c Makefile $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
