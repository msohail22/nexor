FROM debian:bookworm-slim AS build

RUN apt-get update && apt-get install -y \
    g++ make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY Makefile .
COPY src/ src/

RUN make

FROM debian:bookworm-slim

WORKDIR /app
COPY --from=build /app/kvault_app .

CMD ["./kvault_app"]
