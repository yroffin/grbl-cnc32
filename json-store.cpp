#include "json-store.hpp"

JsonStore::JsonStore(JsonDocument &_store) : store(_store)
{
}

int JsonStore::getSize(const char *k1)
{
  if (this->store.containsKey(k1))
  {
    return this->store[k1].size();
  }
  else
  {
    return 0;
  }
}

boolean JsonStore::getAsArray(const char *k1, const char *k2, uint16_t *calData, int size)
{
  if (this->store.containsKey(k1) && this->store[k1].containsKey(k2) && !this->store[k1][k2].isNull())
  {
    for (int i = 0; i < size; i++)
    {
      calData[i] = this->store[k1][k2][i];
    }
    return true;
  }
  else
  {
    return false;
  }
}

void JsonStore::setAsArray(const char *k1, const char *k2, uint16_t *calData, int size)
{
  this->store[k1].createNestedArray(k2);
  for (int i = 0; i < size; i++)
  {
    this->store[k1][k2].add(calData[i]);
  }
}

const char *JsonStore::getAsString(const char *k1, int k2, const char *k3, const char *def)
{
  if (this->store.containsKey(k1) && k2 < this->store[k1].size() && this->store[k1].getElement(k2).containsKey(k3) && !this->store[k1].getElement(k2)[k3].isNull())
  {
    return this->store[k1].getElement(k2)[k3];
  }
  else
  {
    return def;
  }
}

int JsonStore::getAsInt(const char *k1, const char *k2, const char *k3, int def)
{
  if (this->store.containsKey(k1) && this->store[k1].containsKey(k2) && this->store[k1][k2].containsKey(k3))
  {
    return this->store[k1][k2][k3];
  }
  else
  {
    return def;
  }
}

// load store
void JsonStore::load(const char *filename)
{
  // Open file for reading
  File file = SD.open(filename);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(this->store, file);
  if (error)
    log_i("Failed to read file, using default configuration %s", error.c_str());

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}

// get current content
void JsonStore::get(char *buffer, int size)
{
  serializeJson(this->store, buffer, size);
}

// set current content
void JsonStore::set(char *buffer, int size)
{
  StaticJsonDocument<1024> doc;

  // Deserialize the JSON document
  deserializeJson(doc, buffer);
  doc["last_modification"] = millis();

  // set new version
  this->store.set(doc);

  // return new version
  serializeJson(this->store, buffer, size);
}

// save store
void JsonStore::save(const char *filename, const char *old)
{
  // Delete existing file, otherwise the configuration is appended to the file
  SD.remove(old);
  SD.rename(filename, old);

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file)
  {
    log_i("Failed to create file");
    return;
  }

  // Serialize JSON to file
  if (serializeJson(this->store, file) == 0)
  {
    log_i("Failed to write to file");
  }

  // Close the file
  file.close();
}
