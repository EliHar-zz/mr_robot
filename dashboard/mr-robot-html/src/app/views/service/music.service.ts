import { Injectable } from '@angular/core';
import {MusicItem} from "../music-view/music-item/music-item";

@Injectable()
export class MusicService {

  private songs : MusicItem[];
  private playingSong : MusicItem;
  public static DEFAULT_PATH : string = "/assets/";
  public static DEFAULT_IMAGE : string = "song.png";

  public static MUSIC_PLAY:number = 1;
  public static MUSIC_PAUSE:number = 2;
  public static MUSIC_STOP:number = 3;

  private status:number = MusicService.MUSIC_STOP;

  constructor() {
    this.songs = [];
    this.playingSong = null;
  }

  public addSong(song : MusicItem) {
    this.songs.push(song);
  }

  public getSong(id:number) {
    return id >= 0 && id < this.songs.length ? this.songs[id] : null;
  }

  public play(id : number) {
    this.status = MusicService.MUSIC_PLAY;
    this.playingSong = this.getSong(id);
    if(this.playingSong == null) {
      this.stop();
    }
  }

  public stop() {
    this.status = MusicService.MUSIC_STOP;
    this.playingSong = null;
  }

  public pause() {
    this.status = MusicService.MUSIC_PAUSE;
  }

  public get getSongs(){
    return this.songs;
  }

  public get getPlayingSong() {
    return this.playingSong;
  }

  public get getStatus() {
    return this.status;
  }
}
