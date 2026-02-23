library(ggplot2)
library(tidyr)
library(dplyr)

lines = readLines("/home/danielred/data/programs/MCRSbase/OUT/f1np/f1np.data")
pars = lines[1:2]
lines = lines[c(-1, -2)]
table <- read.table(text=lines)
colnames(table) = c("generations", "parazite", "empty", "E1", "E2", "E3")


table |>
  select(!empty)|>
  pivot_longer(!generations) |>
  ggplot(aes(x=generations, y=value))+
  geom_area(aes(fill=name))+
  theme_minimal()+
  labs(title="Geometric mean")

ggsave("/home/danielred/data/alma/andrasnak/2026_02_24/geometric_mean2.png", width=10, height=5)


lines = readLines("/home/danielred/data/programs/MCRSbase/OUT/f2np/f2np.data")
pars = lines[1:2]
lines = lines[c(-1, -2)]
table <- read.table(text=lines)
colnames(table) = c("generations", "parazite", "empty", "E1", "E2", "E3")

table |>
  select(!empty)|>
  pivot_longer(!generations) |>
  ggplot(aes(x=generations, y=value))+
  geom_area(aes(fill=name))+
  theme_minimal()+
  labs(title="Minimum")

ggsave("/home/danielred/data/alma/andrasnak/2026_02_24/minimum2.png", width=10, height=5)

lines = readLines("/home/danielred/data/programs/MCRSbase/OUT/f4np/f4np.data")
pars = lines[1:2]
lines = lines[c(-1, -2)]
table <- read.table(text=lines)
colnames(table) = c("generations", "parazite", "empty", "E1", "E2", "E3")

table |>
  select(!empty)|>
  pivot_longer(!generations) |>
  ggplot(aes(x=generations, y=value))+
  geom_area(aes(fill=name))+
  theme_minimal()+
  labs(title="Flat")

ggsave("/home/danielred/data/alma/andrasnak/2026_02_24/flat2.png", width=10, height=5)


lines = readLines("/home/danielred/data/programs/MCRSbase/OUT/t2/t2.data")
pars = lines[1:2]
lines = lines[c(-1, -2)]
table <- read.table(text=lines)
colnames(table) = c("generations", "parazite", "empty", "E1", "E2", "E3")

table |>
  select(!empty)|>
  pivot_longer(!generations) |>
  ggplot(aes(x=generations, y=value))+
  geom_area(aes(fill=name))+
  theme_minimal()+
  labs(title="Random landscape")

ggsave("/home/danielred/data/alma/andrasnak/2026_02_24/random.png", width=10, height=5)
