import dotenv from "dotenv";
dotenv.config();

export default {
  CONFIG: {
    backendHost: "$VUE_APP_API_URL",
  },
  value(name) {
    const v = this.CONFIG[name];
    if (!v) {
      return;
    }

    if (v.startsWith("$VUE_APP_")) {
      const envName = v.substr(1);
      const envValue = process.env[envName];
      if (envValue) {
        return envValue;
      } else {
        return;
      }
    } else {
      return v;
    }
  },
};
