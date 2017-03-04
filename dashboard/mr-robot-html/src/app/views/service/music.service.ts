import { Injectable } from '@angular/core';
import {MusicItem} from "../music-view/music-item/music-item";

@Injectable()
export class MusicService {

  private songs : MusicItem[];
  private isPlaying : boolean;
  public static DEFAULT_PATH : string = "/assets/";
  public static DEFAULT_IMAGE : string = "song.png";
  constructor() {
    this.songs = [];
  }

  public addSong(song : MusicItem) {
    this.songs.push(song);
  }

  public getSong(id:number) {
    return id >= 0 && id < this.songs.length ? this.songs[id] : null;
  }

  public get getSongs(){
    return this.songs;
  }
}
