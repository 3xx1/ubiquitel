typedef enum DataType{
  SYNC_UB,
  PLAY_UB,
  PAUSE_UB,
  STOP_UB,
  SET_LOOP,
  SET_NOTE,
  RESET_NOTE,
  SEARCH_UB
}DataType;

typedef enum CallbackType{
  UB_FOUND,
  UB_DOCKED,
  UB_UNDOCKED
}CallbackType;

typedef struct Note {
  int ts;//Time stamp(Release point)
  int v;//Velocity
  int sp;//Start point
}Note;

