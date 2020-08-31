# API

Provides a broker between the database and the various clients who need to interact with it

## Endpoints

- `GET /weather?limit={integer}`
  - gets the most recent weather records
- `GET /battery?limit={integer}`
  - gets the most recent device battery records
- `POST /weather`
  - insert a new weather record
- `POST /battery`
  - insert a new battery record

### Entity Definitions

**Battery**

```
{
    id: numeric, autogenerated
    volts: double, required
    createdAt: timestamp, autogenerated
    updatedAt: timestamp, autogenerated
}
```

**Weather**

```
{
    id: numeric, autogenerated
    celsius: double, required
    relative_humidity: double, required
    pressure: double, required
    createdAt: timestamp, autogenerated
    updatedAt: timestamp, autogenerated
}
```

## Running

**Prerequisites**

- Postgres instance available to this API

**Required Environment Variables**

- `PG_USER` - Postgres Username
- `PG_PASS` - Postgres Password
- `PG_URL` - Hostname of Postgres instance
- `PG_PORT` - Postgres Port

**Running this application:**

- `npm install`
- `npm run start`

_please note this API uses Sequelize and will automatically create whatever tables are needed_

**Running via Docker**

```
docker run -p 8080:8080 -e PG_USER=yours -e PG_PASS=yours -e PG_URL=yours -e PG_PORT=yours jvb1993/nodejs-docker-api
```