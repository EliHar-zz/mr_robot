import { Injectable } from '@angular/core';
import {MusicItem} from "../music-view/music-item/music-item";

@Injectable()
export class MusicService {

  private songs : MusicItem[];
  public static DEFAULT_IMAGE : string = "/assets/song.png";
  constructor() {
    this.songs = [];
  }

  public addSong(song : MusicItem) {
    this.songs.push(song);
  }

  public get getSongs(){
    return this.songs;
  }
}
