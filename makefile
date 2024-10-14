# Definições de compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Arquivos de origem e cabeçalho
SRCS = main.cpp utils.cpp simplex.cpp
HEADERS = utils.h simplex.h

# Arquivos objeto
OBJS = $(SRCS:.cpp=.o)

# Nome do executável
EXEC = simplex_solver

# Alvo principal
all: $(EXEC)

# Regra para compilar o executável
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar arquivos objeto
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra de limpeza
clean:
	rm -f $(OBJS) $(EXEC)

# Marcar arquivos objeto como intermediários
.INTERMEDIATE: $(OBJS)

