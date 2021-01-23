#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char expected_header[] = "HL2DEMO";
static const size_t HEADER_LEN = sizeof(expected_header);

static const size_t STRING_LEN = 260;


struct demo_header
{
  char *header;

  int demo_protocol;
  int network_protocol;

  char *server_name;
  char *client_name;
  char *map_name;
  char *game_directory;

  float playback_time;
  int ticks;
  int frames;
  int sign_on_length;
};


struct demo_header read_demo_header(FILE *const file);
void print_demo_header(const struct demo_header demo_header);
void free_demo_header(struct demo_header demo_header);

char *read_string(FILE *const file, const size_t count);
int read_int(FILE *const file);
float read_float(FILE *const file);

void die(const char *const msg);


int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "USAGE: %s <file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *const file = fopen(argv[1], "r");

  if (!file) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }

  struct demo_header demo_header = read_demo_header(file);

  fclose(file);

  print_demo_header(demo_header);

  free_demo_header(demo_header);

  return EXIT_SUCCESS;
}


struct demo_header read_demo_header(FILE *const file)
{
  struct demo_header demo_header;


  demo_header.header = read_string(file, HEADER_LEN);

  if (strcmp(demo_header.header, expected_header) != 0) {
    die("Header was not what was expected");
  }

  demo_header.demo_protocol = read_int(file);
  demo_header.network_protocol = read_int(file);

  demo_header.server_name = read_string(file, STRING_LEN);
  demo_header.client_name = read_string(file, STRING_LEN);
  demo_header.map_name = read_string(file, STRING_LEN);
  demo_header.game_directory = read_string(file, STRING_LEN);

  demo_header.playback_time = read_float(file);
  demo_header.ticks = read_int(file);
  demo_header.frames = read_int(file);
  demo_header.sign_on_length = read_int(file);


  return demo_header;
}

void print_demo_header(const struct demo_header demo_header)
{
  printf(
    "Header: %s\n"
    "Demo Protocol: %d\n"
    "Network Protocol: %d\n"
    "Server name: %s\n"
    "Client name: %s\n"
    "Map name: %s\n"
    "Game directory: %s\n"
    "Playback time: %f\n"
    "Ticks: %d\n"
    "Frames: %d\n"
    "Sign on length: %d\n",
    demo_header.header,
    demo_header.demo_protocol,
    demo_header.network_protocol,
    demo_header.server_name,
    demo_header.client_name,
    demo_header.map_name,
    demo_header.game_directory,
    demo_header.playback_time,
    demo_header.ticks,
    demo_header.frames,
    demo_header.sign_on_length);
}

void free_demo_header(struct demo_header demo_header)
{
  free(demo_header.header);

  free(demo_header.server_name);
  free(demo_header.client_name);
  free(demo_header.map_name);
  free(demo_header.game_directory);
}


char *read_string(FILE *const file, const size_t count)
{
  char *const str = malloc(sizeof(str[0]) * count);

  if (fread(str, sizeof(str[0]), count, file) != count) {
    die("Could not read string");
  }

  if (str[count - 1] != 0) { die("No null terminator"); }


  return str;
}

int read_int(FILE *const file)
{
  int i;

  if (fread(&i, sizeof(i), 1, file) != 1) { die("Could not read int"); }


  return i;
}

float read_float(FILE *const file)
{
  float f;

  if (fread(&f, sizeof(f), 1, file) != 1) { die("Could not read float"); }


  return f;
}


void die(const char *const msg)
{
  fprintf(stderr, "%s\n", msg);

  exit(EXIT_FAILURE);
}
