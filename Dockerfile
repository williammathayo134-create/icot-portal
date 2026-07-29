# Kutumia Alpine Linux kwa ajili ya wepesi na usalama
FROM alpine:latest

# Kusakinisha g++ na zana za C++
RUN apk update && apk add --no-cache g++ make

# Kuweka sehemu ya kazi ndani ya container
WORKDIR /app

# Kuhamisha mafaili yako yote kutoka Termux kwenda kwenye container
COPY . /app

# Kusanya (Compile) faili zako za C++ ziwe server moja thabiti
RUN g++ -O3 server.cpp results_engine.cpp -o icot_server

# Kufungua bandari (Port) 8080 ili iweze kupokea maombi kutoka nje
EXPOSE 8080

# Amri ya kuwasha seva moja kwa moja wakati inapoanza Render
CMD ["./icot_server"]
