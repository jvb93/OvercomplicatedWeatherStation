var express = require("express");
var bodyParser = require("body-parser");
var cors = require("cors");
var jsonParser = bodyParser.json();

var sequelize = require("sequelize");

var app = express();
app.use(cors());
var router = express.Router();

var pg = new sequelize(
  `postgres://${process.env.PG_USER}:${process.env.PG_PASS}@${process.env.PG_URL}:${process.env.PG_PORT}/datalog`
);

pg.authenticate()
  .then(() => {
    console.log("Connection has been established successfully.");
  })
  .catch((err) => {
    console.error("Unable to connect to the database:", err);
  });

const Weather = pg.define("weather", {
  id: {
    type: sequelize.BIGINT,
    autoIncrement: true,
    primaryKey: true,
  },
  celsius: {
    type: sequelize.FLOAT,
  },
  relative_humidity: {
    type: sequelize.FLOAT,
  },
  pressure: {
    type: sequelize.FLOAT,
  },
});

const Battery = pg.define("battery", {
  id: {
    type: sequelize.BIGINT,
    autoIncrement: true,
    primaryKey: true,
  },
  voltage: {
    type: sequelize.FLOAT,
  },
});

Weather.sync();
Battery.sync();

var path = __dirname + "/views/";

// Constants
const PORT = 8080;
const HOST = "0.0.0.0";

router.use(function (req, res, next) {
  console.log("/" + req.method);
  next();
});

router.get("/weather", async (req, res) => {
  try {
    let lim = req.query.limit;
    if (lim == null) {
      lim = 100;
    }
    let weather = await Weather.findAll({
      limit: lim,
      order: [["createdAt", "DESC"]],
    });
    res.json(weather);
  } catch (error) {
    console.error(error);
  }
});

router.post("/weather", jsonParser, (req, res) => {
  try {
    if (req.body.relative_humidity == null || req.body.relative_humidity < 1) {
      res.sendStatus(400);
      return;
    }

    if (req.body.pressure == null || req.body.pressure < 50000) {
      res.sendStatus(400);
      return;
    }

    const newWeather = new Weather(req.body);
    newWeather.save();
    res.sendStatus(201);
  } catch (error) {
    console.error(error);
  }
});

router.get("/battery", async (req, res) => {
  try {
    let lim = req.query.limit;
    if (lim == null) {
      lim = 1;
    }
    let battery = await Battery.findAll({
      limit: lim,
      order: [["createdAt", "DESC"]],
    });
    res.json(battery);
  } catch (error) {
    console.error(error);
  }
});

router.get("/maxbattery", async (req, res) => {
  try {
    let battery = await Battery.findAll({
      limit: 1,
      order: [["voltage", "DESC"]],
    });
    res.json(battery);
  } catch (error) {
    console.error(error);
  }
});

router.get("/minbattery", async (req, res) => {
  try {
    let battery = await Battery.findAll({
      limit: 1,
      order: [["voltage", "ASC"]],
    });
    res.json(battery);
  } catch (error) {
    console.error(error);
  }
});

router.post("/battery", jsonParser, async (req, res) => {
  try {
    if (
      req.body.voltage == null ||
      req.body.voltage < 1000 ||
      req.body.voltage > 4500
    ) {
      res.sendStatus(400);
      return;
    }

    const newBattery = new Battery(req.body);
    newBattery.save();
    res.sendStatus(201);
  } catch (error) {
    console.error(error);
  }
});

app.use(express.static(path));
app.use("/", router);

app.listen(8080, function () {
  console.log("Example app listening on port 8080!");
});
