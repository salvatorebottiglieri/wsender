# Makefile per compilare tutti i file .c presenti nella directory e generare l'eseguibile

# Compilatore da utilizzare
CC = clang

# Opzioni di compilazione
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm -lsep_net -lsep_log -lsep_file -lsep_string

# Flag di debug
ifdef DEBUG
CFLAGS += -g -O0
else
CFLAGS += -O2
endif

# Nome del file sorgente
SRC = $(wildcard core/*/*.c)  $(wildcard core/*.c) 

# Directory per l'output dell'eseguibile
OUTPUT_DIR = ./core/dist
# Nome dell'eseguibile di output
TARGET = $(OUTPUT_DIR)/a.out

# Regola predefinita
all: $(TARGET)

# Regola per compilare i file sorgenti
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Regola per eseguire il programma
run: $(TARGET)
	$(TARGET)

# Regola per pulire i file generati
clean:
	rm -f $(TARGET)

# Regola per eseguire la pulizia e ricompilare
rebuild: clean all

# Regola per compilare in modalità debug
debug:
	$(MAKE) all DEBUG=1